# Filament Release Notes log

**If you are merging a PR into main**: please add the release note below, under the *Release notes
We are chaning the way Vulkan buffers are handled. We need to switch over to a managed (or view-based) model where the data stored inside the object is a proxy to a Vulkan object that can dynamically be swapped around.

**If you are cherry-picking a commit into an rc/ branch**: add the release note under the
appropriate header in [RELEASE_NOTES.md](./RELEASE_NOTES.md).

## Release notes for next branch cut

- samples: samples now have a CLI to select backend api
- materials: sampler now export their type in the material binary [⚠️ **New Material Version**]
- materials: new mutable specialization constants feature. See the [materials documentation](https://google.github.io/filament/Materials.html) for details. [⚠️ **New Material Version**]
