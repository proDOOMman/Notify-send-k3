# -*- coding: utf-8 -*-
"""
Created on Mon Jun 13 16:17:06 2011
# Copyright (C) 2011  Stanislav (proDOOMman) Kosolapov <prodoomman@gmail.com>
"""

import urllib2, trans
import xml.etree.ElementTree as et

USERNAME = 'user'
PASSWORD = 'pass'

NS = '{http://purl.org/atom/ns#}'
auth = urllib2.HTTPBasicAuthHandler()
auth.add_password('New mail feed',
                  'https://mail.google.com',
                  "%s@gmail.com"%USERNAME,
                  PASSWORD)
request = urllib2.build_opener(auth)
urllib2.install_opener(request)

try:
    response = urllib2.urlopen('https://mail.google.com/mail/feed/atom')
except urllib2.URLError, error:
    print "Error: %s"%error.strerror
    exit(1)
xml = et.parse(response).getroot()
total = int(xml.find('%sfullcount' % NS).text)
text = "{:>45}".format("New mails: %s"%total)
for entry in xml.findall('%sentry' % NS):
    author = entry.find('%sauthor'%NS)
    tmp = "%s (%s) "%(unicode(author.find('%sname'%NS).text).encode('trans'),
               unicode(author.find('%semail'%NS).text).encode('trans'))
    if len(tmp) > 45:
        tmp = tmp[:45]
    message = unicode(entry.find('%stitle'%NS).text).encode('trans')
    if len(message) > 45:
        message = message[:45]
    text += "{:>>45}{:<45}".format(tmp,message)
print text
