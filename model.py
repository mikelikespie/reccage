#!/usr/bin/python
from sqlalchemy import create_engine
from sqlalchemy import Table, Column, Integer, String, MetaData, ForeignKey, Float
from sqlalchemy.orm import mapper, relation, backref
from sqlalchemy.ext.declarative import declarative_base

engine = create_engine('postgres://mike@localhost:5432/mike', echo=True)

Base = declarative_base()
class Actor(Base):
    __tablename__ = 'actors'

    name = Column(String, nullable = False, primary_key=True)
    object = Column(String, nullable = False, primary_key=True)
    value = Column(Float, nullable = False)


    def __init__(self, name, object, value):
        self.name = name
        self.object = object
        self.value = value

    def __repr__(self):
        return "Actor(%s, %s, %f)" % (self.name, self.object, self.value)


metadata = Base.metadata
metadata.create_all(engine)


