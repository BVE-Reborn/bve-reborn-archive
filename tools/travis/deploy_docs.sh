#!/bin/bash

eval "$(ssh-agent -s)" # Start ssh-agent cache
chmod 600 tools/travis/travis_ci_rsa # Allow read access to the private key
ssh-add tools/travis/travis_ci_rsa # Add the private key to SSH

ssh $DEPLOY_USER@docs.bvereborn.com "echo sup"
