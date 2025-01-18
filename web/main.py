from flask import Flask
from flask import request
from flask import jsonify
from flask import render_template
import psycopg2
from datetime import datetime
from datetime import date
from datetime import time

app = Flask(__name__)

def connectdb():
    conn = psycopg2.connect(
        host = "127.0.0.1",
        dbname = 'sqltest',
        user = 'sqluser',
        password = '123456',
        port = '5432'
        )
    return conn

def get_last_data(conn):
    cur = conn.cursor()
    cur.execute("select * from dongchazhedata2 order by id desc limit 1")
    rows = cur.fetchall()
    time = rows[0][3]
    date = rows[0][4]

    time = time.strftime('%H:%M:%S')
    date = date.strftime('%Y-%m-%d')
    data = [rows[0][1],rows[0][2],time,date]
    cur.close()
    return data

def close_db(conn):
    conn.close()

def get_all_data(conn):
    cur = conn.cursor()
    cur.execute("select * from dongchazhedata2 order by id")
    rows = cur.fetchall()
    data = rows

    cur.close()

    alldata = []
    for each in data:
        item = {}
        item["温度"] = each[1]
        item["湿度"] = each[2]
        item["时间"] = each[3].strftime('%H:%M:%S')
        item["日期"] = each[4].strftime('%Y-%m-%d')
        alldata.append(item)
    
    return alldata


####################################################### ROUTE ###########################################

@app.route('/')
def hello_main():
    return render_template('index.html')

@app.route("/hello")
def hello_world():
    args = request.args
    print(args)
    return "hello,world!" + args['name']

@app.route("/hellohtml")
def hello_html():
    args = request.args
    print(args)
    return render_template("hello.html",name=args['name'])

@app.route("/getdata")
def getdata():
    header = connectdb()
    data = get_last_data(header)
    close_db(header)
    #print(data)
    return render_template("lastdata.html",w = data[0],h = data[1],t = data[2],d = data[3])

@app.route("/getlastdata")
def getlastdata():
    header = connectdb()
    data = get_last_data(header)
    close_db(header)
    #print(data)
    return jsonify(data)

@app.route("/getalldata")
def getalldata():
    header = connectdb()
    data = get_all_data(header)
    close_db(header)
    print("send data end")
    return jsonify(data)

@app.route("/login")
def login():
    return render_template("login.html")

@app.route("/games")
def games():
    return render_template("games.html")

@app.route("/echarts")
def echarts():
    return render_template("echarts.html")

if __name__ == '__main__':
    app.run(host="0.0.0.0",port=1024,debug='on')