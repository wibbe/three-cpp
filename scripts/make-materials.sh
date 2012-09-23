#!/bin/sh

base_path=$(pwd)

ruby $base_path/scripts/material-generator.rb --path $base_path $base_path/materials/*.rb