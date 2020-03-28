#!/usr/bin/env python

import click
from mjl.anvil import Loader
from mjl.serial import connect


@click.group()
def cli():
    pass

@click.command()
@click.option('--anvil', default='dev.anv', help='Anvil file')
@click.option('--com', help='Serial COM port')
def strike(anvil, com):
    """Setup project to orchestrate anvil files"""
    click.echo('> create file')
    connect(com=com)
    click.echo('> loading {}'.format(anvil))

    l = Loader(anvil)



cli.add_command(strike)


