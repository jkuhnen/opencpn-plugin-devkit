# Git and review workflow

This page defines **DevKit-local conventions**.

## Task contract

A GitHub issue is the implementation contract. Before editing, restate its scope, acceptance criteria, exclusions, upstream facts to verify, and runtime tests required. Any material scope change belongs in the issue or PR discussion.

## Branches

Create a dedicated branch from current `main`:

```text
feature/<issue>-<short-topic>
fix/<issue>-<short-topic>
docs/<issue>-<short-topic>
codex/<issue>-<short-topic>     # agent-authored work where useful
```

Use lowercase, hyphens, and a concise topic. One branch should solve one issue.

## Commits

Prefer small, reviewable commits with an imperative subject:

```text
docs: add plugin packaging validation
build: align Windows target metadata
fix: bound overlay hover queries
```

Do not commit generated build trees, secrets, signing material, private charts, or machine-specific configuration. Do not bypass hooks without recording and justifying it.

## Implementation and validation

1. Read the issue and relevant DevKit docs.
2. Verify OpenCPN facts against current authoritative sources.
3. Implement on the dedicated branch while preserving unrelated user changes.
4. Review the complete diff for scope, secrets, generated files, and unsupported claims.
5. Run formatting/static checks, configure/build, tests, packaging checks, and artifact inspection appropriate to the change.
6. Perform a manual OpenCPN runtime test whenever behavior, ABI, rendering, packaging, DPI, scheme, provider, or lifecycle integration is involved.
7. Clearly list validation not performed.

## Pull request

Open the PR against `main`. The description should link the issue, summarize behavior and architecture, cite important upstream decisions, list commands/checks and results, enumerate runtime environments tested, and call out risks or remaining manual checks.

ChatGPT/Codex may implement and review mechanics, but a human reviews the diff and any safety/UX interpretation. Merge only after required review and runtime testing. Never merge merely because CI is green.

## History safety

No force push, destructive reset, branch deletion, or history rewrite without explicit instruction. Prefer additive fixes and normal follow-up commits during review. If a rebase is requested, confirm the exact branch and preserve recoverability.
