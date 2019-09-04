{% if cookiecutter.create_git_repo == 'Y' -%}
import subprocess

subprocess.call(['git', 'init'])
subprocess.call(['git', 'add', '*'])
subprocess.call(['git', 'commit', '-m', 'Initial commit'])

{% endif %}