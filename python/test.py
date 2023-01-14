import os
import sys
import json

check_result = {
    'is_error': False,
    'is_warning': False,
    'error': [],
    'warning': [],
    'succeed': []
}

white_list = []


def error_handler(error, warning):
    if error:
        check_result['is_error'] = True
        check_result['error'].append(error)

    if warning:
        check_result['is_warning'] = True
        check_result['warning'].append(warning)


def succeed_handler(succeed_msg):
    check_result['is_warning'] = False
    check_result['is_error'] = False
    check_result['warning'].append("")
    check_result['error'].append("")
    check_result['succeed'].append(succeed_msg)



def on_error(name, error):
    if name in white_list:
        error_handler(None, error)
        return

    error_handler(error, None)


if __name__ == '__main__':
    succeed_msg = "succeed_msg"
    succeed_msg = "succeed - '{}'\n".format(200)
    succeed_handler(succeed_msg)
    print(json.dumps(check_result))