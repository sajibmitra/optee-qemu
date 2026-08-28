# Local contributions

## tinyml

OP-TEE TinyML sample (host CA + TA). Sources live here so they are tracked in
this repo; copy or symlink into `optee_examples/tinyml` when building with the
stock examples package layout:

```bash
ln -sfn "$(pwd)/contrib/tinyml" optee_examples/tinyml
```

Then rebuild Buildroot / examples as usual from `build/`.
