from flask import Flask, request, jsonify
from flask_cors import CORS
import subprocess
import os

app = Flask(__name__)
CORS(app)

@app.route('/compile', methods=['POST'])
def compile_code():
    code = request.json.get('code')

    # Write editor code directly to test.cpp
    with open("test.cpp", "w") as test_file:
        test_file.write(code)

    try:
        # Run hello.exe with test.cpp
        subprocess.run(['hello.exe', 'test.cpp'], capture_output=True, text=True, timeout=10)

        # Read lexical analysis from tokens.txt
        lexical = "tokens.txt not found"
        if os.path.exists('tokens.txt'):
            with open('tokens.txt', 'r') as f:
                lexical = f.read()

        # Read assembly code from compile.txt
        assembly = "compile.txt not found"
        if os.path.exists('compile.txt'):
            with open('compile.txt', 'r') as f:
                assembly = f.read()

        return jsonify({
            'lexical': lexical.strip(),
            'assembly': assembly.strip(),
        })

    except Exception as e:
        return jsonify({'error': str(e)}), 500

if __name__ == '__main__':
    app.run(debug=True)
