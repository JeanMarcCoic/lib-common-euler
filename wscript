##########################################################################
#                                                                        #
#  Copyright (C) INTERSEC SA                                             #
#                                                                        #
#  Should you receive a copy of this source code, you must check you     #
#  have a proper, written authorization of INTERSEC to hold it. If you   #
#  don't have such an authorization, you must DELETE all source code     #
#  files in your possession, and inform INTERSEC of the fact you obtain  #
#  these files. Should you not comply to these terms, you can be         #
#  prosecuted in the extent permitted by applicable law.                 #
#                                                                        #
##########################################################################
# pylint: disable = invalid-name

import os

# pylint: disable = import-error
import waflib
# pylint: enable = import-error

out = '.build-waf-%s' % os.environ.get('P', 'default')

# {{{ options

def options(ctx):
    ctx.recurse('lib-common')

# }}}
# {{{ configure

def configure(ctx):
    ctx.recurse('lib-common')

# }}}
# {{{ build

def build(ctx):
    ctx.recurse(['lib-common', 'problems'])
    ctx.program(target='hello-world', source='hello-world.c',
                use=['libcommon'])

# }}}
