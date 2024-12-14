#!/usr/bin/env python
import os
import csv
import io
import pika
import json

from flask import Flask, request, jsonify
from pymongo import MongoClient

app = Flask(__name__)

client = MongoClient("mongodb-data-service:27017")
database = client["weatherDB"]
collection = database["weather"]


rabbit_connection = pika.BlockingConnection(
  pika.ConnectionParameters(host='rabbitmq-service',connection_attempts=10, retry_delay=5)
)

rabbit_channel = rabbit_connection.channel()

rabbit_channel.queue_declare(queue='weather_analysis')


def load_init_data():
  if collection.estimated_document_count() == 0:
    with open('measurements.csv', newline='') as csvfile:
      reader = csv.DictReader(csvfile)
      records = [row for row in reader]
      collection.insert_many(records[:100])
      global latest_date
      latest_date=records[:100][-1]['Date']
  else:
    result = collection.find_one(sort=[("Date", -1)], projection={"Date": 1, "_id": 0})
    latest_date=result["Date"]

@app.route('/')
def check_connection():
  try:
    return "Welcome to WeatherData!\n"

  except Exception as e:
    raise Exception("The following error occurred: ", e)


@app.route('/data', methods=['GET'])
def get_data():
  try:
    date = request.args.get('date')

    if date:
      data = list(collection.find({"Date": date}, {'_id': 0}))
      if not data:
        return jsonify({"message": "No data."}), 404
      return jsonify(data), 200

    else:
      data = list(collection.find({}, {'_id': 0}))
      return jsonify(data), 200

  except Exception as e:
    return jsonify({"error": str(e)}), 500


@app.route('/data', methods=['POST'])
def upload_data():
  try:
    data = request.get_json()
    if not data:
      return jsonify({"error": "Request body must contain JSON data."}), 400

    required_fields = {"Date","Time","SO2", "NO2","O3", "CO", "PM10", "PM2.5"  }
    if not required_fields.issubset(data.keys()):
      return jsonify({"error": f"Missing fields: {required_fields - set(data.keys())}"}), 400

    if len(list(collection.find({"Date": data['Date'],"Time": data['Time']}, {'_id': 0})))>=1:
      collection.replace_one({"Date": data['Date'],"Time": data['Time']},data)
      return jsonify({"message": "Data replaced"}), 201

    else:
      collection.insert_one(data)

      current_date=data['Date']
      global latest_date

      if current_date>latest_date:
        so2=[]
        no2=[]
        o3=[]
        co=[]
        pm10=[]
        pm2_5=[]
        result={}

        all_measurements=list(collection.find({"Date": latest_date}, {'_id': 0}))

        for el in all_measurements:
          so2.append(float(el['SO2']))
          no2.append(float(el['NO2']))
          o3.append(float(el['O3']))
          co.append(float(el['CO']))
          pm10.append(float(el['PM10']))
          pm2_5.append(float(el['PM2.5']))
        
        result = {
          "Date": latest_date,
          "SO2": str(sum(so2) / len(all_measurements)),
          "NO2": str(sum(no2) / len(all_measurements)),
          "O3": str(sum(o3) / len(all_measurements)),
          "CO": str(sum(co) / len(all_measurements)),
          "PM10": str(sum(pm10) / len(all_measurements)),
          "PM2.5": str(sum(pm2_5) / len(all_measurements)),

        }

        rabbit_channel.basic_publish(exchange='',
                      routing_key='weather_analysis',
                      body=json.dumps(result))

        latest_date=current_date
        return jsonify({"message": "Data added and analysis done"}), 201


      return jsonify({"message": "Data added"}), 201

  except Exception as e:
      return jsonify({"error": str(e)}), 500

if __name__ == "__main__":
  load_init_data()
  app.run(host='0.0.0.0', port=os.environ.get("FLASK_SERVER_PORT", 9090), debug=True, use_reloader=False)
  rabbit_connection.close()