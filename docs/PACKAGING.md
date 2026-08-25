# Plugin packaging and distribution

## Authority

**Upstream fact.** The current [`OpenCPN/plugins`](https://github.com/OpenCPN/plugins) repository publishes the plugin catalog, schema, metadata sources, and validation tools. OpenCPN's plugin-installer documentation defines tarball concepts and layout. Working plugin repositories generate artifacts using more than one build family.

**DevKit convention.** Treat packaging as a separate deliverable with its own validation. Compilation success is necessary but never sufficient.

## Metadata source and generated metadata

A plugin project may maintain a `metadata.xml.in` template and generate a concrete `metadata.xml` or target-specific XML during packaging. The exact names and generation step come from the selected proven build infrastructure.

The current catalog schema includes:

- identity: `name`, `version`, `release`, `summary`;
- compatibility: `api-version`, `target`, optional `build-target`/`build-gtk`, `target-version`, `target-arch`;
- provenance: `open-source`, `author`, `source`, optional `info-url`, `description`;
- delivery: `tarball-url` and optional `tarball-checksum` under the schema rules.

Do not assume ordering, requiredness, allowed values, or compatibility matching from this summary. Validate the generated file against the current XSD and compare it with current catalog entries for the same target family.

## Version and compatibility fields

- **Plugin version/release:** must agree across source, generated metadata, archive name/content, tag, and release notes.
- **API version:** identifies the plugin API used to build the binary; select it from the actual header/library, not the OpenCPN application version.
- **Target/build-target:** compatibility identifiers interpreted by plugin infrastructure. Preserve distinctions such as wxWidgets target variants.
- **Target architecture:** describe the produced binary, not the build machine.
- **Target Windows version:** derive from the verified build policy/toolchain and current metadata conventions; do not guess from the developer workstation.

## Package creation

Maintained build families may use CPack with a TGZ generator and additional tarball/metadata post-processing. Use the documented target in that repository. Do not rename a raw CPack archive and assume it is plugin-manager ready; some workflows modify the archive after CPack.

## Tarball inspection

Inspect the final uploaded candidate, not an intermediate file:

```powershell
tar -tf <artifact.tar.gz>
tar -xzf <artifact.tar.gz> -C <empty-inspection-directory>
```

Confirm:

- paths conform to the current plugin-installer tarball specification;
- the expected plugin binary exists once, with correct architecture and dependencies;
- required metadata is present in the location/name expected by the selected workflow;
- resources, translations, licenses, and debug symbols are present only as intended;
- there are no source-tree paths, build caches, credentials, signing files, private charts, or unrelated binaries;
- archive names and internal metadata agree.

## Catalog and distribution

The current `OpenCPN/plugins` project accepts target-specific metadata through pull requests and validates it against its schema. Its documented FE2 flow uses Cloudsmith repositories for artifacts and metadata in applicable plugin projects. Other maintained infrastructure may publish differently.

Cloudsmith is therefore **current infrastructure where the selected workflow uses it**, not a DevKit requirement and not a universal OpenCPN API behavior. Repository names, channels, credentials, upload commands, and catalog branches must come from the plugin's current release process. Never embed upload tokens in the repository.

When URLs/checksums are part of the workflow:

- upload the immutable final artifact;
- compute/record the checksum from those exact bytes;
- verify the URL anonymously where public access is intended;
- validate redirects, content length/type, and checksum;
- ensure alpha/beta/production channel and catalog destination match release intent.

## Validation checklist

1. Build and package from a clean checkout with initialized submodules.
2. Confirm version/API/target fields against CMake output and binary inspection.
3. Validate XML with the current `OpenCPN/plugins` XSD/tooling.
4. Inspect the final tarball tree and extracted contents.
5. Verify URL and checksum after upload, if applicable.
6. Import the tarball using the target OpenCPN Plugin Manager where supported.
7. Test install, enable, basic use, disable, uninstall, and restart.
8. Confirm the plugin is offered only to compatible hosts in a test/private catalog when catalog matching changed.
9. Retain build logs, generated metadata, checksums, and artifacts for review.
10. Record all untested host/architecture combinations.

Never publish merely because compilation or CPack succeeded.
