#!/usr/bin/env python

import click


@click.group()
def cli():
    pass

@click.command()
@click.option('--anvil', default='dev.anv', help='Anvil file')
@click.option('--com', help='Serial COM port')
def strike(anvil, com):
    """Setup project to orchestrate anvil files"""
    click.echo('> building {}'.format(anvil))


cli.add_command(strike)


