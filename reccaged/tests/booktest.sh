#!/bin/bash

# This example is from the book Programming Collective Intelligence by Toby Segaran

host=$1


curl "$host?method=add&actor=Lisa%20Rose&object=Lady%20in%20the%20Water&value=2.5"
curl "$host?method=add&actor=Lisa%20Rose&object=Snakes%20on%20a%20Plane&value=3.5"
curl "$host?method=add&actor=Lisa%20Rose&object=Just%20My%20Luck&value=3.0"
curl "$host?method=add&actor=Lisa%20Rose&object=Superman%20Returns&value=3.5"
curl "$host?method=add&actor=Lisa%20Rose&object=You,%20Me%20and%20Dupree&value=2.5"
curl "$host?method=add&actor=Lisa%20Rose&object=The%20Night%20Listener&value=3.0"

curl "$host?method=add&actor=Gene%20Seymour&object=Lady%20in%20the%20Water&value=3.0"
curl "$host?method=add&actor=Gene%20Seymour&object=Snakes%20on%20a%20Plane&value=3.5"
curl "$host?method=add&actor=Gene%20Seymour&object=Just%20My%20Luck&value=1.5"
curl "$host?method=add&actor=Gene%20Seymour&object=Superman%20Returns&value=5.0"
curl "$host?method=add&actor=Gene%20Seymour&object=The%20Night%20Listener&value=3.0"
curl "$host?method=add&actor=Gene%20Seymour&object=You,%20Me%20and%20Dupree&value=3.5"

curl "$host?method=add&actor=Michael%20Phillips&object=Lady%20in%20the%20Water&value=2.5"
curl "$host?method=add&actor=Michael%20Phillips&object=Snakes%20on%20a%20Plane&value=3.0"
curl "$host?method=add&actor=Michael%20Phillips&object=Superman%20Returns&value=3.5"
curl "$host?method=add&actor=Michael%20Phillips&object=The%20Night%20Listener&value=4.0"

curl "$host?method=add&actor=Claudia%20Puig&object=Snakes%20on%20a%20Plane&value=3.5"
curl "$host?method=add&actor=Claudia%20Puig&object=Just%20My%20Luck&value=3.0"
curl "$host?method=add&actor=Claudia%20Puig&object=The%20Night%20Listener&value=4.5"
curl "$host?method=add&actor=Claudia%20Puig&object=Superman%20Returns&value=4.0"
curl "$host?method=add&actor=Claudia%20Puig&object=You,%20Me%20and%20Dupree&value=2.5"

curl "$host?method=add&actor=Mick%20LaSalle&object=Lady%20in%20the%20Water&value=3.0"
curl "$host?method=add&actor=Mick%20LaSalle&object=Snakes%20on%20a%20Plane&value=4.0"
curl "$host?method=add&actor=Mick%20LaSalle&object=Just%20My%20Luck&value=2.0"
curl "$host?method=add&actor=Mick%20LaSalle&object=Superman%20Returns&value=3.0"
curl "$host?method=add&actor=Mick%20LaSalle&object=The%20Night%20Listener&value=3.0"
curl "$host?method=add&actor=Mick%20LaSalle&object=You,%20Me%20and%20Dupree&value=2.0"

curl "$host?method=add&actor=Jack%20Matthews&object=Lady%20in%20the%20Water&value=3.0"
curl "$host?method=add&actor=Jack%20Matthews&object=Snakes%20on%20a%20Plane&value=4.0"
curl "$host?method=add&actor=Jack%20Matthews&object=The%20Night%20Listener&value=3.0"
curl "$host?method=add&actor=Jack%20Matthews&object=Superman%20Returns&value=5.0"
curl "$host?method=add&actor=Jack%20Matthews&object=You,%20Me%20and%20Dupree&value=3.5"

curl "$host?method=add&actor=Toby&object=Snakes%20on%20a%20Plane&value=4.5"
curl "$host?method=add&actor=Toby&object=You,%20Me%20and%20Dupree&value=1.0"
curl "$host?method=add&actor=Toby&object=Superman%20Returns&value=4.0"

curl "$host?method=getTopSimilar&actor=Lisa%20Rose"
curl "$host?method=getRecs&actor=Toby"


