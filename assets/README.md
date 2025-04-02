# Assets
For Dungeon Crawler Jam 2025 we want to be as portable as possible while also offering
an easy cross-platform workflow.

For this reason we're using [battery/embed](https://github.com/batterycenter/embed/) to
embed all of our assets into C++ byte arrays.

As we will most likely end up making lo-fi art we won't surpass filesizes of a few MBs per
file.

If this will ever change - and we will have larger files above >50MB per file we should
consider a hybrid approach: embedding the small assets, having a load-from-disk approach
for larger files!

For emscripten we could then utilize the ```-preload-file``` compile flag.

For desktop builds we would need to decide upon ways to deal with external files.

Either simply copying over the assets to the binary folder or some sort of container /
packing format similar to Epic's .pak. These would also come handy for patching via
itch.io and Steam. E.g. [SteamPipe prefers pack files with similar layouts where only 
the relevant segments change](https://partner.steamgames.com/doc/sdk/uploading) - this is ideal for offering low download payloads to the end
users.

Another consideration would be how to guarantee the file integrity using hashes/checksums
to make sure the game assets are unchanged / as expected.

But this is something to worry about when it becomes relevant...