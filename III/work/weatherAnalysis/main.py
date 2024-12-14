#!/usr/bin/env python
import os
import pika
import threading
import json

from flask import Flask, request, jsonify
from pymongo import MongoClient
import sys

app = Flask(__name__)

client = MongoClient("mongodb-analysis-service:27017")
database = client["weatherAnalysesDB"]
collection = database["weatherAnalsys"]

rabbit_connection = pika.BlockingConnection(
  pika.ConnectionParameters(host='rabbitmq-service', connection_attempts=10, retry_delay=5)
  )

rabbit_channel = rabbit_connection.channel()
rabbit_channel.queue_declare(queue='weather_analysis')

def callback(ch, method, properties, body):
  print(f"Data received {json.loads(body)}\n", file=sys.stderr)
  collection.insert_one(json.loads(body))


rabbit_channel.basic_consume(queue='weather_analysis',
                      auto_ack=True,
                      on_message_callback=callback)

@app.route('/')
def check_connection():
  try:
    client.admin.command('ismaster')
    return "Welcome to WeatherDataAnalysis!\n"

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
      return jsonify(data[0]), 200

    else:
      data = list(collection.find({}, {'_id': 0}))
      return jsonify(data), 200

  except Exception as e:
    return jsonify({"error": str(e)}), 500



if __name__ == "__main__":
  threading.Thread(target=lambda: app.run(host='0.0.0.0', port=os.environ.get("FLASK_SERVER_PORT", 9091),
   debug=True, use_reloader=False)).start()

  rabbit_channel.start_consuming()