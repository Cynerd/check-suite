#!/usr/bin/env bash
set -eu

version="${CI_COMMIT_TAG#v}"
if ! pysemver check "$version"; then
	echo "Tag has to be valid semantic version!" >&2
	exit 1
fi

# Changelog should contain as a first section this release as this is the
# latest release.
changelog="$(awk -v "version=$version" '
		BEGIN {
			flag = 0
		}
		/^## / {
			if ( $0 !~ "^## \\[" version "\\]" ) {
				exit
			}
			flag = 1
			next
		}
		/^## \[/ && flag {
			exit
		}
		flag {
			print
		}
	' CHANGELOG.md)"
if [ -z "$changelog" ]; then
	echo "Changelog is empty." \
		"Have you updated the version? Is this the latest version?" >&2
	exit 1
fi

release-cli create \
	--name "Release $version" \
	--tag-name "$CI_COMMIT_TAG" \
	--description "$changelog"
