#!/bin/bash
git pull
read -r commit
git add .
git commit -m "$commit"
git push
