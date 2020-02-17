
virtualenv --python=python3 /tmp/.venv 

source /tmp/.venv/bin/activate

git clone https://github.com/adafruit/Adafruit_Python_BluefruitLE.git /tmp/adafruit

cd /tmp/adafruit

python setup.py install

cd -

pip install -r requirements.macosx.txt 