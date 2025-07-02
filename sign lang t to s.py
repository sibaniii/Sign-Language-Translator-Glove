#this code i ran i used to convert the detected messages to speech (run on laptop and attach laptop to a speaker)
import serial
import pyttsx3

# initialize text-to-speech engine
engine = pyttsx3.init()
engine.setProperty('volume', 1.0)  # volume range: 0.0 to 1.0

# connect to arduino (ensure the COM port matches your setup)
arduino = serial.Serial('COM6', 9600, timeout=1)

spoken_last = ""

while True:
    try:
        line = arduino.readline().decode('utf-8').strip()

        if "Phrase:" in line:
            # extract phrase after "Phrase:"
            phrase = line.split("Phrase:")[-1].strip()

            # speak only if the phrase is non-empty and not a repeat
            if phrase and phrase != spoken_last:
                print(f"Speaking: {phrase}")
                engine.say(phrase)
                engine.runAndWait()
                spoken_last = phrase

    except Exception as e:
        print("Error:", e)
