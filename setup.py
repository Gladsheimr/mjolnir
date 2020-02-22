#!/usr/bin/env python

from setuptools import setup, find_packages

setup(name='mjolnir',
      version='0.0.2',
      description='Command line tools to orchestrate gladsheimr projects',
      author='Kartik Thakore',
      author_email='admin@maitrise.ai',
      url='https://github.com/Gladsheimr/mjolnir',
      py_modules=['mjl'],
      packages=find_packages(),
      include_package_data=True,
      install_requires=[
        'Click',
      ],
      entry_points='''
        [console_scripts]
        mjl=mjl.scripts.mjl:cli
        ''',
     )
