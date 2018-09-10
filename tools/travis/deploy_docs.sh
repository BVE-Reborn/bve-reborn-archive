#!/bin/bash

eval "$(ssh-agent -s)" # Start ssh-agent cache
ls ${TRAVIS_BUILD_DIR}/tools/travis/
chmod 600 ${TRAVIS_BUILD_DIR}/tools/travis/travis_ci_rsa # Allow read access to the private key
ssh-add ${TRAVIS_BUILD_DIR}/tools/travis/travis_ci_rsa # Add the private key to SSH

ssh $DEPLOY_USER@docs.bvereborn.com "echo sup"
