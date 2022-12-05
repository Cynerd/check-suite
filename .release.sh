#!/usr/bin/env bash
set -eu

VERSION="$(echo "${CI_COMMIT_TAG}" | sed -nE 's/v([0-9]+)\.([0-9]+)\.([0-9]+).*/\1.\2.\3/p')"
if [ -z "$VERSION" ]; then
	echo "Tag has to start with valid version number such as v1.0.0!" >&2
	exit 1
fi

CHANGELOG="$(awk -v "version=${CI_COMMIT_TAG#v}" '
        BEGIN {
            flag = 0
        }
        ( $0 ~ "^## \\[" version "\\]" ) {
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
if [ -z "$CHANGELOG" ]; then
    echo "Changelog is empty. Have you updated the version?" >&2
    exit 1
fi

declare -a args
while read -r dist; do
	URL="${CI_API_V4_URL}/projects/${CI_PROJECT_ID}/packages/generic/release/${VERSION}/${dist##*/}"
	echo "$dist -> $URL"
	curl --header "JOB-TOKEN: ${CI_JOB_TOKEN}" --upload-file "${dist}" "${URL}"
	args+=("--assets-link" "{\"name\":\"${dist}\",\"url\":\"${URL}\"}")
done <<<"$(yq '.dist.artifacts.paths[]' .gitlab-ci.yml)"

release-cli create \
	--name "Release ${CI_COMMIT_TAG#v}" \
	--tag-name "$CI_COMMIT_TAG" \
	--description "$CHANGELOG" \
	"${args[@]}"
