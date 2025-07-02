import serial
import pyttsx3

# Initialize text-to-speech engine
engine = pyttsx3.init()
engine.setProperty('volume', 1.0)  # Volume range: 0.0 to 1.0

# Connect to Arduino (ensure the COM port matches your setup)
arduino = serial.Serial('COM6', 9600, timeout=1)

spoken_last = ""

while True:
    try:
        line = arduino.readline().decode('utf-8').strip()

        if "Phrase:" in line:
            # Extract phrase after "Phrase:"
            phrase = line.split("Phrase:")[-1].strip()

            # Speak only if the phrase is non-empty and not a repeat
            if phrase and phrase != spoken_last:
                print(f"Speaking: {phrase}")
                engine.say(phrase)
                engine.runAndWait()
                spoken_last = phrase

    except Exception as e:
        print("Error:", e)
