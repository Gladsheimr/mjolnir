#!/usr/bin/env python

from distutils.core import setup

setup(name='mjolnir',
      version='0.0.1',
      description='Command line tools to orchestrate gladsheimr projects',
      author='Kartik Thakore',
      author_email='gladheimr@gmail.com',
      url='https://github.com/Gladsheimr/mjolnir',
      py_modules=['mjl'],
      install_requires=[
        'Click',
      ],
      entry_points='''
        [console_scripts]
        mjl=mjl:cli
        ''',
     )
