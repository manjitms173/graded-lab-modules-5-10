# VI RECOVERY MECHANISMS - ANSWER

## THE QUESTION
Developer edits file in vi. System crashes before save. Which recovery method is most reliable?

## THE ANSWER
**SWAP FILES (vi -r filename) - Score: 9/10**

## WHY SWAP FILES ARE BEST

1. **Automatic** - No setup needed, enabled by default
2. **Built for crashes** - Designed to survive system failures
3. **95-99% recovery** - Gets almost all your work back
4. **One command** - Just type: vi -r filename
5. **Works everywhere** - All Linux/Unix systems

## HOW TO RECOVER

```bash
vi -r filename
:w!
rm .filename.swp
ALL 5 MECHANISMS RANKED

1. Swap Files - 9/10 ⭐

    Works after crash: YES
    Data recovered: 95-99%
    Setup needed: NO

2. Auto-Save - 6/10

    Works after crash: YES
    Data recovered: Depends on save interval
    Setup needed: YES

3. Backup Files - 4/10

    Works after crash: YES
    Data recovered: Only last saved version
    Setup needed: YES

4. Undo History - 2/10

    Works after crash: NO (RAM lost)
    Data recovered: 0%
    Setup needed: YES

5. Registers - 1/10

    Works after crash: NO (RAM lost)
    Data recovered: 0%
    Setup needed: NO

EXAMPLE

Before crash:

max_connections=100

Made changes:

max_connections=150
pool_size=20

CRASH

Recovery with swap file:

vi -r database.conf

Result:

max_connections=150  ✓ RECOVERED
pool_size=20        ✓ RECOVERED

Recovery with backup:

cp database.conf.backup database.conf

Result:

max_connections=100  ✗ CHANGES LOST

WHY OTHERS FAIL

Undo & Registers: Stored in RAM. Crash kills vim process = data lost forever.

Backups: Only have last saved state. Unsaved changes = gone.

Auto-Save: Not a recovery method, just saves more often. Still loses data.
CONCLUSION

Swap files win because they:

    Work with zero setup
    Survive any crash type
    Recover 95-99% of work
    Battle-tested 30+ years

Command to remember: vi -r filename

