#!/bin/bash

eval "$(ssh-agent -s)" # Start ssh-agent cache
chmod 600 ${TRAVIS_BUILD_DIR}/tools/travis/travis_ci_rsa # Allow read access to the private key
ssh-add ${TRAVIS_BUILD_DIR}/tools/travis/travis_ci_rsa # Add the private key to SSH

ssh -o "StrictHostKeyChecking no" $DEPLOY_USER@docs.bvereborn.com "echo sup"
rsync -e "ssh -o StrictHostKeyChecking=no" -az ${TRAVIS_BUILD_DIR}/../docs-build/html/ $DEPLOY_USER@docs.bvereborn.com:$DEPLOY_LOCATION/$TRAVIS_BRANCH/