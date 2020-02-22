#!/usr/bin/env python

import click
from mjl.anvil import Loader
from mjl.serial import connect

from cookiecutter.main import cookiecutter


@click.group()
def cli():
    pass

@click.command()
def init():
    """Setup project template"""
    print('Creating project folder')
    cookiecutter('https://github.com/Gladsheimr/mjolnir-cookiecutter.git')

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



