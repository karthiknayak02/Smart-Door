# This app is pretty straightforward: It takes the user's speech and passes it to the braille translator app.

from flask import Flask, render_template
from flask_ask import Ask, statement, question, session
import picamera

user_resp = ""

app = Flask(__name__)   # Setup: Our basic app instance.

ask = Ask(app, "/") # Setup: Our Amazon Skills Kit instance

@ask.launch
def prompt_text():  # What is asked on application launch
    input_prompt = render_template('prompt')    # "prompt" is stored in templates.yaml
    return question(input_prompt)

def pic():  
    camera = picamera.PiCamera()
    camera.capture('image.jpg')
    return

@ask.intent("MessageIntent", convert={'response':str})
def get_text(response): # Gets the user's message, passes it into the another program.
    # Some function that translates the response will go here
    global user_resp

    #if user_resp == "":
    isRight = render_template('confirmStatement', response=response)
    user_resp = response
    text_file = open("alexa_output.txt", "w")
    text_file.write("%s" % user_resp)
    text_file.close()
    pic()
    return statement(render_template('correct'))


if __name__ == '__main__':
     app.run(debug=False)
     
     
     
     
