import json
import os
from pathlib import Path
import subprocess
import tempfile
import html

ROOT = Path.cwd()
tests = json.loads((ROOT / '.github/classroom/autograding.json').read_text())['tests']
score = 0
maximum = sum(int(t['points']) for t in tests)

for test in tests:
    name = test['name']
    points = int(test['points'])
    try:
        with tempfile.TemporaryDirectory() as temp:
            wd = Path(temp)
            # Test setup writes main.c in the isolated directory; student bst.c and DS.h are copied in.
            for filename in ('bst.c', 'DS.h'):
                (wd / filename).write_bytes((ROOT / filename).read_bytes())
            setup = subprocess.run(['bash', '-eo', 'pipefail', '-c', test['setup']], cwd=wd,
                                   capture_output=True, text=True, timeout=60)
            if setup.returncode != 0:
                raise RuntimeError('setup failed: ' + (setup.stderr or setup.stdout)[-600:])
            run = subprocess.run(['bash', '-eo', 'pipefail', '-c', test['run']], cwd=wd,
                                 input=test.get('input', ''), capture_output=True, text=True,
                                 timeout=max(15, int(test.get('timeout', 1)) + 10))
            if run.returncode != 0:
                raise RuntimeError('run failed: ' + (run.stderr or run.stdout)[-600:])
            actual = run.stdout
            expected = test.get('output', '')
            # Normalize CRLF produced by GitHub Classroom JSON input/output.
            actual = actual.replace('\r\n', '\n')
            expected = expected.replace('\r\n', '\n')
            if actual != expected:
                raise RuntimeError(f'output mismatch (expected {len(expected)} chars, got {len(actual)} chars)')
            score += points
            print(f'PASS {name}: +{points}', flush=True)
    except Exception as exc:
        print(f'FAIL {name}: +0/{points}: {exc}', flush=True)

print(f'SCORE: {score}/{maximum}', flush=True)
badge = ROOT / '.github/badges/points.svg'
badge.parent.mkdir(parents=True, exist_ok=True)
value = f'{score}/{maximum}'
color = '#2da44e' if score == maximum else '#e3b341' if score else '#d73a49'
badge.write_text(f'''<svg xmlns="http://www.w3.org/2000/svg" width="126" height="20" role="img" aria-label="Points: {html.escape(value)}">
<rect width="126" height="20" rx="3" fill="#555"/>
<path d="M66 0h57a3 3 0 0 1 3 3v14a3 3 0 0 1-3 3H66z" fill="{color}"/>
<g fill="#fff" font-family="Verdana,DejaVu Sans,sans-serif" font-size="11" text-anchor="middle"><text x="33" y="14">Points</text><text x="96" y="14">{html.escape(value)}</text></g>
</svg>''', encoding='utf-8')
# Nonzero exit if at least one test fails; badge is still committed by the workflow.
raise SystemExit(0 if score == maximum else 1)

