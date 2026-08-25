# Agent guidance

This is an OpenCPN plugin repository. `.devkit` is the development-only shared playbook; it is not a runtime, build, or package dependency. Before changing OpenCPN API behavior, architecture, Windows builds, packaging, maritime HMI, design-system rules, CI, or Git workflow, read `.devkit/AGENTS.md` and the relevant `.devkit/docs/*` files.

Use this precedence:

```text
verified upstream OpenCPN/API behavior
        ↓
issue/task-specific requirements
        ↓
this plugin's documented rules and proven implementation
        ↓
shared DevKit conventions
        ↓
agent assumptions
```

If `.devkit` is unavailable, initialize the pinned submodules using `README.md`; do not fabricate missing guidance. Do not silently update either `.devkit` or `opencpn-libs` during unrelated work.

## Plugin defaults

- Keep C++11 and OpenCPN plugin API 1.18 unless a dedicated compatibility task changes the baseline after upstream verification.
- Keep the OpenCPN adapter thin. Put domain/data/query logic and wx UI in explicit modules only when the plugin needs them.
- Use only documented upstream OpenCPN/plugin APIs. Do not add experimental/private APIs, provider-specific chart parsing, or proprietary chart dependencies.
- Keep callbacks bounded and non-blocking. No network, device, or slow file work belongs in UI/render callbacks.
- Preserve clean ownership and teardown. Remove host integrations and stop owned work before plugin destruction.
- Centralize UI colors/metrics and follow OpenCPN DAY/DUSK/NIGHT. Maritime guidance is not evidence of ECDIS, regulatory, or type-approval compliance.
- Use a dedicated branch, keep changes scoped, review the diff, validate configure/build/package as applicable, and open a PR against `main` without merging it unless explicitly instructed.
- Never commit build output, credentials, signing material, private charts, tokens, or machine-specific secrets.
- Report exact checks and identify any OpenCPN runtime/platform validation still required.
