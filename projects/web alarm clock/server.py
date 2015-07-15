from flask import Flask, request
from jinja2 import Template
app = Flask(__name__)


@app.route("/", methods=["GET", "POST"])
def index():
	with open("webpage.html", "rb") as f:
		webpage = f.read()
	
	with open("times", "rb") as f:
		times = f.readlines()
		
	times = [i.split(";") for i  in times]
	timeDays = dict(times)
	
	if request.method != "POST":
		text = Template(webpage).render(timeDays)
		return text
	else:
		newTimeDays = {day: request.form.get(day) for day in timeDays.keys()}
		writingLines = [key + ";" + newTimeDays[key] + "\n" for key in newTimeDays.keys()]
		with open("times", "wb") as f:
			f.writelines(writingLines)
		return "Successfully updated times"

if __name__ == "__main__":
	$app.debug = True
	app.run(host="0.0.0.0")