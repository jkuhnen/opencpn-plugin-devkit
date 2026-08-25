# Agent guidance

This repository is an unofficial OpenCPN plugin development playbook. Before changing a plugin or this DevKit:

- Read the relevant files in `docs/` and the curated sources in `references/README.md`.
- Treat the current OpenCPN source, developer documentation, target plugin API headers, `opencpn-libs`, and official/community plugin infrastructure as authoritative. Verify version-sensitive behavior there; never invent OpenCPN APIs or callbacks.
- Label verified upstream facts, task assumptions, and DevKit-local conventions distinctly.
- Preserve the target plugin's C++, wxWidgets, CMake, and OpenCPN conventions unless the task explicitly changes them.
- Work on a dedicated branch and hand changes over through a pull request. Avoid force pushes, history rewrites, and destructive Git operations unless explicitly authorized.
- Do not commit generated build output, credentials, private chart data, signing material, tokens, or machine-specific paths and secrets.
- State which automated checks ran and clearly identify any validation that still requires a real OpenCPN runtime, platform, chart/provider, or device.
- Treat maritime HMI material as design guidance only. Never infer ECDIS, regulatory, type-approval, or standards compliance without the applicable certification and evidence.
