let editor;

// Load Monaco Editor
require.config({ paths: { vs: 'https://unpkg.com/monaco-editor@latest/min/vs' } });
require(['vs/editor/editor.main'], function () {
  editor = monaco.editor.create(document.getElementById('editor'), {
    value: '// Write your C++ code here\n',
    language: 'cpp',
    theme: 'vs-dark'
  });
});

// Compile the code and display results
function compileCode() {
    const code = editor.getValue();

    fetch('http://127.0.0.1:5000/compile', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ code })
    })
    .then(res => res.json())
    .then(data => {
      document.getElementById('lexical-output').innerText = data.lexical || 'No output';
      document.getElementById('assembly-output').innerText = data.assembly || 'No output';
    })
    .catch(err => alert('Compilation error: ' + err));
}

