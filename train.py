#!/usr/bin/python

from sys import argv, stderr

file = argv[1]

f = open(file)

object = f.readline()[0:-2]

for l in f:
    spl = l.split(',')
    user = spl[0]
    value = spl[1]


    print "%s\t%s\t%s" % (user, object, value)

#    a = session.query(Actor).\
#        filter(Actor.name == user).\
#        filter(Actor.object == object).first()
#    if a is None:
#    else:
#        a.value = float(value)

stderr.write("committed %s\n" % object)

