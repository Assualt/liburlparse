import sys
import os
sys.path.append(os.path.dirname(__file__) + '/../cmake-build-debug/python')
import pyurlparse as urlparse
if __name__ == '__main__':
    ret = urlparse.add(1, 2)
    print(ret)

    urls = urlparse.parse_url("https://www.baidu.com")
    print(urls)

    pass