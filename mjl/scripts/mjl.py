#!/usr/bin/env python

import os
import click

from mjl.anvil import Loader
from mjl.serial import connect

import mjl.templates

from cookiecutter.main import cookiecutter

cookiecutter_root = mjl.templates.__path__[0]

@click.group()
def cli():
    pass

@click.command()
def init():
    """Setup project template"""
    print('Creating project folder')
    project_cookiecutter_dir = os.path.join(cookiecutter_root, 'project')
    print('Found cookiecutter {}', project_cookiecutter_dir)
    cookiecutter(project_cookiecutter_dir)

@click.command()
@click.option('--anvil', default='dev.anv', help='Anvil file')
@click.option('--com', help='Serial COM port')
def strike(anvil, com):
    """Setup project to orchestrate anvil files"""
    click.echo('> create file')
    connect(com=com)
    click.echo('> loading {}'.format(anvil))



cli.add_command(init)
cli.add_command(strike)



