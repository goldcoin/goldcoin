# Goldcoin Development Configuration

## Git Repository Setup
- **Development Fork**: https://github.com/microguy/goldcoin.git
- **Upstream Original**: https://github.com/goldcoin/goldcoin.git
- **Always push to**: `microguy` remote (the fork)
- **Pull updates from**: `upstream` remote if needed

## Development Workflow
- Work on `development` branch
- Push all changes to `microguy/goldcoin` repository
- Create PRs from fork to upstream when ready
- Test GitHub Actions in the fork environment

## Build Commands
- `./autogen.sh && ./configure --disable-wallet --disable-tests --disable-gui && make -j$(nproc)`
- For testing: `./configure --disable-wallet --disable-gui && make -j$(nproc) && make check`
- Use ccache for faster builds
- Test workflows trigger on push to development branch

## Recent Development Status
- **Headers Fixed**: Added missing `<condition_variable>`, `<mutex>`, `<thread>` headers
- **Test Fixes Applied**: Fixed duplicate txin detection and RPC JSON validation tests (commit 876bfe92f)
- **GitHub Actions**: Working on resolving remaining test failures in CI environment

## Security Notes
- This is cryptocurrency software - audit all changes carefully
- Never commit private keys or sensitive data
- Follow defensive security practices