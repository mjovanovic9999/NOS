FROM python:3.13.0-alpine3.20

WORKDIR /src

COPY requirements.txt requirements.txt
RUN pip install -r requirements.txt

COPY . .

EXPOSE 9090

CMD ["python3", "main.py"]