#!/bin/sh

base_path=$(pwd)

ruby $base_path/scripts/material-generator.rb --path $base_path/src/material $base_path/materials/*.rb