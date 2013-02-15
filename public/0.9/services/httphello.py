#!/usr/bin/env python

def application(environ, start_response):
    status = '200 OK'
    response_headers = [('Content-type', 'text/plain')]
    start_response(status, response_headers)
    return ['Hello world!\n'] + ['{0}={1}\n'.format(k,environ[k])
                                 for k in sorted(environ)]

if __name__ == '__main__':
    from wsgiref.simple_server import make_server
    from os import environ
    s = make_server('', int(environ.get('PORT_WWW', '8080')), application)
    print 'Starting server on port {0}.'.format(s.server_port)
    s.serve_forever()
