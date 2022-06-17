# SWS Install Instructions

Software must be build on Raspberry pi hardware.

1. Run `build_sws_release.sh` to build release binary.
1. Make `~/sws` directory
1. Copy `build_release/sws` to `~/sws`
1. Copy `run_sws.sh` to `~/sws`
1. Edit `run_sws.sh` and replace the user 'pi' with your user name in the paths.
1. Create a cron job (`crontab -e`) with the following line, replacing `<USER>` with your user:

```
*/5 * * * * /home/<USER>/sws/run_sws.sh
```

This will run sws every 5 minutes. Use `*` in the first place to run every minute.

## Optional log trimming:

1. Copy `trim_sws_log.sh` to `~/sws`

1. Edit `trim_sws_log.sh` and replace the user 'pi' with your user name in the paths.

Create a cron job with the following to trim the log file every saturday at midnight:

```
0 0 * * 6 /home/<USER>/sws/trim_sws_log.sh
```
