#!/usr/bin/env python
# -*- coding: utf-8 -*-

import urllib2, xml.dom.minidom, trans, subprocess

# For ID's look at http://informer.gismeteo.ru/getcode/xml.php
city_ids = [ '34214_1', '34009_1' ]

line_length = 45 # notify-send define, don't change

def Num2Cloud(num):
    if num == '0':
        return 'fair'
    elif num == '1':
        return 'fair with some cloud'
    elif num == '2':
        return 'cloudy'
    elif num == '3':
        return 'overcast'
    else:
        return "unknown"

def Num2Precip(num):
    if num == '4':
        return 'rain'
    elif num == '5':
        return "rainfall"
    elif num == '6':
        return 'snow'
    elif num == '7':
        return 'snow'
    elif num == '8':
        return 'thunderstorm'
    elif num == '9':
        return 'unknown'
    elif num == '10':
        return 'without precipitation'
    else:
        return "unknown"

def Num2Weekday(num):
    if num == '1':
        return "sunday"
    elif num == '2':
        return "monday"
    elif num == '3':
        return "tuesday"
    elif num == '4':
        return "wednesday"
    elif num == '5':
        return "thursday"
    elif num == '6':
        return "friday"
    elif num == '7':
        return "saturday"
    else:
        return "unknown"

if not subprocess.check_output(["lipc-get-prop","com.lab126.wan","powerState"]) == "1\n":
    print "Please, enable WIFI first"
    exit(1)

#user-agent = "Mozilla/4.0 (compatible; Linux 2.6.22) NetFront/3.4 Kindle/2.2"
#proxy = "fints-g7g.amazon.com"
#proxy = "https://firs-ta-g7g.amazon.com/FirsProxy"
#proxy = urllib2.ProxyHandler({'http': proxy})
#opener = urllib2.build_opener(proxy)
#urllib2.install_opener(opener)

text = ("{:>%s}"%line_length).format("Gismeteo")
for c_id in city_ids:
    cityurl = "http://informer.gismeteo.ru/xml/%s.xml"%c_id
    try:
        page = urllib2.urlopen(cityurl)
    except urllib2.URLError, error:
        print "Error: %s"%error.strerror
        exit(2)
    data = page.read()
    xmlobj = xml.dom.minidom.parseString(data)
    text += ("{:=^%s}"%line_length).format(urllib2.unquote(xmlobj.getElementsByTagName("TOWN")[0].
    getAttribute("sname")).encode('iso8859-15').decode('cp1251').encode('trans'))

    for node in xmlobj.getElementsByTagName("FORECAST"):
        date = "%s.%s.%s (%s) %s:00"%(node.getAttribute("day"),
                                 node.getAttribute("month"),
                                 node.getAttribute("year"),
                                 Num2Weekday(node.getAttribute("weekday")),
                                 node.getAttribute("hour"))
        t_max = node.getElementsByTagName("TEMPERATURE")[0].getAttribute("max")
        t_min = node.getElementsByTagName("TEMPERATURE")[0].getAttribute("min")
        cloud = Num2Cloud(node.getElementsByTagName("PHENOMENA")[0]
                                .getAttribute("cloudiness"))
        precip = Num2Precip(node.getElementsByTagName("PHENOMENA")[0]
                                .getAttribute("precipitation"))
        temp = "{:<%s}{:^%s}"%(line_length,line_length)
        text += temp.format(date,"%s %s, %s..%s C"%(cloud,precip,t_min,t_max))

print text
