# Goldcoin Rust Implementation Validation Checklist

## 1. Consensus Rules
- [ ] Block size limits (32MB max)
- [ ] Transaction size limits
- [ ] Coinbase maturity (100 blocks)
- [ ] Script validation (P2PKH, P2SH, multisig)
- [ ] Signature verification (ECDSA)
- [ ] UTXO set management
- [ ] Double-spend prevention
- [ ] Block timestamp validation
- [ ] Merkle root verification

## 2. Goldcoin-Specific Features
- [x] Protocol version 70015
- [x] Golden River difficulty algorithm
- [x] 51% Defense System
- [x] Automatic checkpointing with dropout
- [x] Treasury block at height 1,905,100
- [ ] Script address support (starting with 'g')
- [ ] Legacy address support (starting with 'G')

## 3. Hard Forks
- [x] julyFork (45,000)
- [x] novemberFork (103,000)  
- [x] novemberFork2 (118,800)
- [x] mayFork (248,000)
- [x] julyFork2 (251,230)
- [x] febFork (372,000)
- [ ] Block subsidy schedule changes at each fork
- [ ] Difficulty adjustment changes at each fork

## 4. Block Subsidies
- [x] Genesis: 50 GLC
- [x] Blocks 1-200: 10,000 GLC
- [x] Blocks 201-2,200: 1,000 GLC
- [x] Blocks 2,201-44,999: 500 GLC
- [x] Post-julyFork: Smooth halving formula
- [x] Treasury block: 1,100,000,000 GLC
- [ ] Correct subsidy calculation at all heights

## 5. Network Protocol
- [x] P2P port 8121 (mainnet)
- [x] RPC port 8122 (mainnet)
- [x] DNS seed discovery
- [ ] Version handshake
- [ ] Block propagation
- [ ] Transaction relay
- [ ] Peer management
- [ ] Ban score system

## 6. Transaction Validation (txindex=1)
- [ ] Input script execution
- [ ] Output script validation
- [ ] Witness data handling (if applicable)
- [ ] Fee calculation
- [ ] Size/weight limits
- [ ] Sequence number checks
- [ ] Locktime validation
- [ ] RBF (Replace-By-Fee) if supported

## 7. Memory Pool
- [ ] Transaction priority
- [ ] Fee-based eviction
- [ ] Orphan transaction handling
- [ ] Chain limits
- [ ] Ancestor/descendant limits
- [ ] Memory pool persistence

## 8. Storage & Database
- [ ] Block storage (.dat files)
- [ ] Block index
- [ ] UTXO database
- [ ] Transaction index (when txindex=1)
- [ ] Chainstate management
- [ ] Proper flushing on shutdown

## 9. RPC Interface
- [ ] getblockchaininfo
- [ ] getbestblockhash
- [ ] getblock
- [ ] getrawtransaction
- [ ] sendrawtransaction
- [ ] getmininginfo
- [ ] Mining pool specific RPCs

## 10. Mining Pool Requirements
- [x] txindex=1 support
- [ ] getblocktemplate
- [ ] submitblock
- [ ] Block validation before propagation
- [ ] Coinbase transaction construction
- [ ] Queued block support (51% defense)

## 11. Critical Edge Cases
- [ ] Reorg handling
- [ ] Fork resolution
- [ ] Time warp attack prevention
- [ ] Malformed transaction rejection
- [ ] Resource exhaustion prevention
- [ ] Integer overflow checks

## 12. Testing Required
- [ ] Sync from genesis to current height
- [ ] Validate against C++ daemon
- [ ] Submit test transactions
- [ ] Mine test blocks on testnet
- [ ] Stress test with high transaction volume
- [ ] Test all hard fork transitions