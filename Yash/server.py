from flask import Flask, request, jsonify
from flask_cors import CORS
import subprocess
import tempfile
import os

app = Flask(__name__)
CORS(app)

@app.route('/compile', methods=['POST'])
def compile_code():
    code = request.json.get('code')

    with tempfile.NamedTemporaryFile(delete=False, suffix=".cpp", mode='w') as temp_file:
        temp_file.write(code)
        temp_path = temp_file.name

    try:
        result = subprocess.run(['hello.exe', temp_path], capture_output=True, text=True, timeout=10)
        output = result.stdout.split('---SPLIT---')
        print("Raw Output from hello.exe:\n", result.stdout)

        # Ensure output list has at least 3 elements
        output += [""] * (3 - len(output))

        return jsonify({
            'lexical': output[0].strip(),
            'semantic': output[1].strip(),
            'assembly': output[2].strip(),
        })
    except Exception as e:
        print("Error during compilation:", e)
        return jsonify({'error': str(e)}), 500
    finally:
        os.remove(temp_path)

if __name__ == '__main__':
    app.run(debug=True)
