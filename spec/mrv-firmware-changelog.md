# MRV ALAMO/ALTO firmware changelogs

The firmware changelogs that MRV Systems supplied with the ALAMO/ALTO float firmware, collected from
`Dropbox/mrv/firmware`. The text is as supplied, newest first, except that e-mail addresses are
removed. The firmware version a float runs is in its mission configuration (`MIS` line, `fwver`;
see [listing-file-format.md](listing-file-format.md)).

| Section | Versions | Source file |
|---|---|---|
| [VOP firmware 8.0.4 – 10.13.0](#vop-firmware-804--10130) | 2020-12 – 2025-10 | `10.13.0+wims/ChangeLog` |
| [VOP firmware 8.0.0 – 8.0.3](#vop-firmware-800--803) | 2020-05 – 2020-11 | `ChangeLog-10.0.1` (history entries) |
| [MFA firmware 1.5.a – 4.5.d](#mfa-firmware-15a--45d) | 2014 – 2017-04 | `MRV Firmware Architecture_MFA Revision History (1).txt` |

The other files in that directory hold subsets of these:

- `ChangeLog-10.5.0`, `ChangeLog-5-23-2022`, `ChangeLog_10.4.6`, `ChangeLog_10.6.1`, `ChangeLog_10.6.3`,
  `ALTO-ALAMO-10.6.2/10.6.2_ChangeLog`, `10.12.1+wims/ChangeLog` and `ChangeLog-10.0.1` are earlier copies of
  the cumulative VOP ChangeLog. They differ only by line wrapping and one blob name ("FF/CA" for "CA").
- `VOP ALAMO Software Revision History.txt` (1.5a – 1.7b), `VOP ALAMO Software Revision History
  CTD_2916_v20h_xA68C.txt` and `changelog_VOP_RBR_PT_2973_v30e_x7EE3.txt` are earlier copies of the MFA
  history. `MRV Firmware Architecture_MFA Revision History.txt` (up to 4.3.a) has one bullet the newer copy
  lacks, under 4.3.a: "removed redundant timestamps from mission configuration packet (0xDn)".
- The PDF changelogs (`ChangeLog-v7.2.2.pdf`, `ChangeLog-v7.3.5.pdf`, `v7-2-1_notes.pdf`) are not included.

None of the changelogs names the retransmission commands `TDD` and `TDP`; the entries that affect them
are listed in [xmessage-format.md C.2](xmessage-format.md#retransmission-requests-tdd-tdp).

## VOP firmware 8.0.4 – 10.13.0

```text
2025-10-27  Stuart Maclean

        * Release 10.13.0

        BUILDS = wimsenc

        A build for floats with WIMS boards that also need data encryption.
        RBR L3 ctd assumed.

        BUILDS = sbe3 sbe3enc

        Fix capacity of Allocator object used by command parsing logic.
        Was too small for SBE3-based ctd builds.

2025-10-23  Stuart Maclean

        * Hotfix 10.12.1

        BUILDS = wims

        Fix serial passthrough to WIMS board. Needed due to recent update
        to WIM shell cmd which leaves uart Tx pin LO at end, as per WIMS
        v2 requirements.

        This hotfix is a WIMS-only build.

2025-10-13  Stuart Maclean

        * Release 10.12.0

        BUILDS = wims
        
        * Add WIMS v2 processing

        * WIMS data encoding fix (Issue 156)

        BUILDS = All
        
        * DOX channels (3) in surface sampling. Needs a decoder that knows of
        new use of 0x88 .. 0x8F blobs.

        * Surface sampling bug fix (Issue 154) - would be cut short if gps
        time < ascent profile retrieval time

        * xmsgfs bug fix (Issue 155) - re-purpose oldest blocks rather than
        recently used ones.
        

2025-03-05  Stuart Maclean

        * Release 10.11.0

        BUILDS = microAlto (ualto)

        Added 'mapl cycle' command to verify that RSI MAPL board boots up
        correctly.

        This release ONLY includes the ualto build.
        
2024-12-24  Stuart Maclean

        * Release 10.10.0

        BUILDS = All

        XMessage/SBD data priorities, allowing pilot-settable order of
        data upload. Individual data blobs may also be disabled/enabled
        from upload at all. New commands are xfs 30, xfs 31, xfs 32 (also
        xfs 8 for testing, creates dummy blobs).

        BUILDS = microAlto (ualto)

        maplconf file setting now persists across (warm) reboot, including
        pwr2 + magswipe.
        
        BUILDS = All H2G builds, inc DMON

        Fix bug resulting in float reboot due to receipt of malformed
        guest-2-host messages. 

2024-12-03  Stuart Maclean

        * Release 10.9.1

        BUILDS = dmon

        Per customer request, now use two gpio pins for DMON control, was
        one. Now use a power pin AND a pulse pin.

        BUILDS = All

        Re-instate the CTD query that obtains initial pressure reading,
        shortly after boot.  This pressure value P decides whether to BIST
        or not. If P < MTD, we bist, else go to mission mode and commence
        next dive. The pressure query was removed in error.
        
2024-10-21  Stuart Maclean

        * Release 10.9.0

        BUILDS = dmon

        New logic for waking DMON guest. Uses a gpio pulse to wake DMON
        guest from low-power mode. Frequency of this pulse maintained by
        DMON mision parameter (units are minutes). We first pulse on entry
        to park phase, and then every DMON minutes. A new DMON-only
        command 'DMP' enables user-initiated gpio pulse.

        BUILDS = All

        A PDT value of 0 no longer causes park phase to end immediately on
        entry to park.  It did prior.

        New XFS 21 + 22 commands for xferring, via xmodem, all pending
        (21) or pending+transmitted (22) SBD data, to console.  Enables
        offload of stored xmessage data without need for Iridium.

        The release build date printed on boot now user-defined, was
        toolchain/build-date-defined.

        
2024-04-04  Stuart Maclean

        * Release 10.8.0

        BUILDS = wims

        Streamline the WIMS magnetic calibration procedure. In the WIM
        command, user now simply selects options for zeroing params,
        starting/stopping cal and installing new 6-param set from a menu,
        rather than copious manual entry of WIMS commands.

        BUILDS = All

        Addition of VAC console-only for monitoring case vacuum over time.

        Improvement to BIST, prints more details to console as it progresses.

        Improvement to BIST, notes 'traveler incomplete' as a BIST
        error/warning.

        Add 5 sec pause after AT+SBDI failure when commanding SBD
        modem from both BIST and SND command. Surface-phase logic for modem
        interaction unchanged.


2024-03-07  Stuart Maclean

        * Hotfix 10.7.2

        BUILDS = rbr4enc, sbe3enc

        For the with-data-encryption builds, the PWD function fixed so
        that the password can be of ANY length, up to 31 chars.  In prior
        versions, attempting to set a password of length 4N (for some int
        N), e.g. '9834', would lead to that password being stored
        incorrectly, with subsequent password checks failing.

2024-02-08  Stuart Maclean

        * Release 10.7.1

        BUILDS = All

        On receipt of 'pwr 2' command, whether via console or Iridium,
        power off Iridium modem and GPS receiver before entering low-power
        mode.

2024-01-22  Stuart Maclean

        * Release 10.7.0

        BUILDS = All

        Remove call to u-blox GPS requesting space vehicle info.  Suspect the
        dynamic nature of the reply, sometimes very large, could be leading
        to situations whereby no gps fix info sent home.

        Ensure ctd ascent profile data is stored even when float ends dive
        ascent early due to ice-turnaround. Done for both RBR and SBE ctd
        types. NOTE: Still issues with correctness of CST, CRD, CRS
        parameter usage.

        Fix logic in burtc-peripheral-as-clock whereby clock could jump
        backwards by large amount.

        Add alkaline battery type details to traveler data, as used by BIST.

        In ice algorithm, alter default for ice urgency mode exit to be
        data threshold based, was absolute (and unattainable!).

        Add xfs 7 and 8 commands, the first shows all pending data blobs,
        the second a debug tool for creating stored data at will.

        BIST now includes requesting the CTD (RBR only) to commence
        profiling, and inspects and reports results.
        

2023-06-21  Alex Loper

    * Release 10.6.3

    BUILDS = All

    Improve serial device passthrough mode to now also interact with
    devices on J5,J6 expansion headers. The passthrough is now exited
    via a special escape sequence, enabling richer passthrough
    conversations interfacing to e.g. RBR's Ruskin.

    BUILDS = All

    Mission triggering, via the MTD and MTS mission parameters, now uses
    elapsed time only, and does not consult local clock. Local clock
    can be changed by a BIST, which includes GPS.
    GH issue 119

    BUILDS = All

    Accommodate unsolicited outputs of the Iridium SBD modem,
    e.g. +CIEV, when interfacing with the modem.
    GH issue 120

    BUILDS = All with RBR CTD

    No longer send postprocessing commands to the RBR CTD.  RBR
    firmware, as of version 1.1.47, is confused by postprocessing
    commands sent to it, and produces incorrect dynamically corrected
    salinity values.

2023-05-19  Stuart Maclean

        * Release 10.6.2

        Nominally a hotfix rather than release, but IS started from develop.
        Just fixes two issues present up to 10.6.1.

        BUILDS = All with sbe3

        Park pressure and temperature series now assigned to correct
        XMessage blobs, were swapped in error.

        BUILDS = All with rbr4

        Issue 'memclear' to the RBR CTD in surface phase, after profile
        retrieval and any surface sampling.  memclear ensures that any
        later retrieval of profile data from the RBR would not result in
        duplicate data being retrieved.

2023-04-28  Stuart Maclean

        * Release 10.6.1

        BUILDS = ualto

        All file transfers, including MAPL board firmware update, uses a
        timeout mechanism to recognize that the file transfer is complete.
        In 10.6.0, an escape character sequence was used.  It was not
        intended to be a release solution.

2023-04-14  Alex Loper

        * Release 10.6.0 [not built via git flow, gh release]

        BUILDS = All
        
        - fixed bug in bist where the start time was incorrect

        - added oil pump low current warning to bist

        - fixed bug in traveler where it would accept invalid SSNs
        
        BUILDS = ualto

        - added support for updating MAPLe boards through the vop

        BUILDS = All that use an RBR CTD

        - added PPE to mission state dump

        - removed PPR command and rolled it into PPE. PPE now has 3 enums:
        off, enabled, enabled with raw salinity included

        - fixed bug in rbr profile retrieval where a crc failure would cause
        the profile retrieval to end prematurely

        - added new crc failure debug packet to 0xff packets

        - improved ctd fetch console formatting

        - added support for ODO channels with RBR

2023-02-27  Stuart Maclean

        * Release 10.5.0

        BUILDS = All

        Redesigned BIST execution and results presentation. XMessage BIST
        blob (E5/E6) updated to reflect BIST enhancements.
        
        New 'traveler' data captures e.g. air pump type, accessed via new
        TRV command.

        BUILDS = All H2G builds (altoxb-jhu, altoxb-nuwc, bmfx, dmon)

        Add three new messages from host to guest: $HGRST, $HGBIT, $HGMIS.

        BUILD = rsi

        Initial mapl conf file now 'mapl.cnf', was empty.
        Preamble no longer issues 'quit'.

2022-11-21  Stuart Maclean

        * Release 10.4.6

        Could be seen as a hotfix, since only really adding fixes to four
        issues (bugs) found in 10.4.4 and later, compared to 10.2.2 and
        earlier.  Issues are documented as nextgen_a Issues 93-96:

        93: pre-dive GPS blob (id=0x01) tagged with wrong dive number

        94: ctd status field in engineering surface blob (id=0xea)
        reversed, was 'asid' should be 'disa' (short for disabled).

        95: ctd samples taken during during surface phase stored at SSI
        interval, should be at SDS.
 
        96: timestamps of ctd samples taken during park phase should be
        offset to park start absolute time, were being saved as absolute
        time in error.
        
2022-10-21  Stuart Maclean

        * Hotfix 10.4.5

        An Iridium-received command that itself wants to use modem, such
        as SAT, no longer leaves modem inaccessible. Previously, SAT
        reconfigured modem in such a way that main surface phase could no
        longer use modem.

        Unknown if SAT command ever WAS received, but ssn=11329 exhibits
        SAT-sourced data suggesting such.  

2022-09-15  Stuart Maclean

        * Hotfix 10.4.4

        Up to 10.4.3, mission parameters that took on negative values
        (e.g. SDC = -10) were packed into D0/D1 blobs incorrectly as 0. A
        C2 decoder would thus see SDC=0.

        Further, any sampling of the RBR CTD for pressure would NOT apply
        sdc offset (absolute to sea pressure) until first surface phase OR
        pilot setting sdc.

        Both issues fixed in 10.4.3. No new logic/functionality in 10.4.4.

2022-09-12  Stuart Maclean

        * Release 10.4.3

        Realized that the 10.4.2 release mentioned directly below clashes
        in name with a previous 10.4.2 binary that was given to WHOI for
        testing.  Thus, bumping OUR 10.4.2 to 10.4.3.  In fact, IS correct
        procedure since this code has an addition to rbr interfacing code
        not present in previous 10.4.2 tagged code.

        * Release 10.4.2

        BUILDS: All except Host2Guest ones (altoxb bmfc dmon dmonrbr)

        Internal: Code refactor relating to conversatons with the RBR CTD,
        especially in the area of post-processing.

        Internal: Removed __DATE, __TIME macros, which precluded
        repeatable builds
        (hash of .bin file constantly changing even when code hadn't).

        Internal: Amalgamated N copies of stringMakePrintable into one,
        general clean up.

        NB: 10.4.1 an internal (test?) release that was not
        finished/pursued.
        
2022-07-01  Stuart Maclean

        * Release 10.4.0

        BUILD = rsi

        RSI MAPL board interfacing improvements (10.1.0) as requested by
        RSI: (1) MAPL board powered off on 'pwr 2' (2) baud rate to MAPL
        board reverts to 115200 when MAPL board powered on (3) MAPL baud
        rate settable via top-level 'mapl' command.

        Also included are mapl.txt and maplconf.txt, documenting user
        interface w MAPL board and file transferral.

2022-05-23  Stuart Maclean

        * Release 10.3.0

        BUILD = mm2

        Interface with installed WHOI acoustic modem (micromodem2) and
        Seascan RTC to receive acoustic messages from Marginal-Ice-Zone
        (MIZ) beacons on a schedule. Received messages are stored in
        eeprom and sent home via Iridium.  New CA blob type for this
        data. New mize, mizs, mizd, mizn and miz commands.

2022-05-19  Stuart Maclean

        * Hotfix 10.2.2

        All builds: Command Parser Fix. Parser now handles an input string
        consisting of whitespace chars plus Enter. System fails (reboots
        due to NULL pointer dereference) in all earlier builds.

2022-05-17  Stuart Maclean

        * Hotfix 10.2.1

        All builds: Packet ID (pid) fields in all SBD messages for a given
        dive are now unique.  Previous release (10.2.0) would re-use
        highest PID, leading to possible issues in decoding.


2022-01-21  Stuart Maclean

        * Release 10.2.0

        All builds: fix possible buffer overflow caused by GPS svinfo
        payload being unexpectedly large. The overflow led to system
        lockup, a form of reset.

        RSI build: add log motor messages, sent to MAPL boards when oil
        pump actuated, during ascent.

        Encryption build: Key manipulation command renamed KEY, was KES.
        Key check function, was sub-cmd of KES, now standalone KCF.  All
        key manipulation commands now fall under an administrator role.
        Access to that role is protected by password. New PWD command
        added for password setting. KEY change via console causes all
        local dive data stored in EEPROM to be wiped.

        
2021-10-29  Stuart Maclean

        * RC 10.1.0-mapl (aka rc1)

        BUILD = rsi

        Initial RSI MAPL board integration (MAPL guest on J5). Files
        retrieved from MAPL via xModem and stored as xmsg blobs (id=0xF1).
        Corresponding FileAssembler tool shore-side re-assembles original
        files.

        Includes 10.0.1 updates, dated later.

2021-12-02  Stuart Maclean

        * Release 10.0.1. Essentially a hotfix to address failure of
        command parser to update pilot-set parameters during BIST (BIT
        command) and via SND command.

2021-11-23  Stuart Maclean

        * Release 10.0.0, builds for rbr4, sbe3, rbr4odo, altoxb, bmfc,
        dmon, rbr4-SIM-C (simulated RBR CTD), rbr4-SIM-CGI (simulated RBR
        plus simulated GPS, Iridium modem).

        - Initial console baud rate is 9600.

        - BAUD command added, to alter console baud (to e.g. 115200).
        This command only recognized via cabled console (NOT Bluetooth).

        - DTB (Device Tool Box) command added, for device diagnostics over
        Iridium. DTB command can write arbitrary strings to GPS, Iridium,
        CTD and any guest devices (J5,J6) and harvest replies.  Device
        power on/off/query capabilities too. DTB results sent home in new
        FF/DB SBD data blobs.

        - XFS (XMessage File System) command added, for inspection,
        manipulation of SBD data stored in non-volatile storage (eeprom).
        This is the float data that is routinely sent home via Iridium
        SBD. The re-organization of the stored data, accompanying this XFS
        command, is such that Iridium-pending data is not lost if the
        float reboots

        - For floats equipped with RBR CTDs, the float now recognizes and
        tallies any NaN values emitted by the CTD. These are brought home
        via Iridium SBD in new FF/EC data blobs.

        - Improved mission parameter manipulation. Current value of each
        parameter can be printed at console.  The 'glob' character allows
        sets of parameters to be printed at once, e.g. d*, pt*. Existing MIC
        commands are unaffected.

        - BIT and SND commands have improved Iridium modem interaction.

2021-10-26  Stuart Maclean

        * RC 10.0.0-rc7

        - Re-instate XAS and XM commands, omitted in error during command parser
        re-org.

        - Fix PST command parameter range, should be max of 36000, was set at
        3600 in error.

2021-10-21  Stuart Maclean

        * RC 10.0.0-rc6

        - Add RBR NaN tallying and SBD xmission.  NaNs may be present in
        readdata responses.

2021-10-15 Christian Sarason

        * RC 10.0.0-rc+rbr4odo (reconstructed from history)

        - Integrated rbr O2 channels in profiling and surface phase 
        from previous version (7.3.0) into latest float controller code.

2021-10-15 Christian Sarason

        * RC 10.0.0-rc3+rbr4 (reconstructed from history)

        - Improved data storage so that data collected during profiling 
        persists across float reset or brownout. (major change)

        - Updated command parser for both console and iridium processing 
        to allow for non-integer (and longer) arguments.

2021-06-16 Christian Sarason

        * RC 9.0.0-rc1+rbr4 (reconstructed from history)

        - Implements revised dive-numbering logic if a float resets.
        
        - Divenumbers will increase (always). 
        
        - BIST numbers will decrease (e.g. -1, -2, -3, ... for subsequent BISTs). 

        - Initial diagnostic dive will retain cycle number 0. 

        - If the float resets subsequently in the field, future diagnostic 
        dives may occur(depends on the nature of the reset) but cycle number 
        will keep increasing and the dive will appear to be performing a 
        "strange" dive profile.

2021-06-02 Christian Sarason

        * RC 8.1.0.rc7 (reconstructed from history)

        - Fix watchdog reset in surface phase of RBR equipped ALXXOs, 
        reproduced on test bench setup in April 2021.
        
        - Add engineering “capture packet” in 0xFF XMessage packet. 
        This packet is MRV internal engineering data, and is thus 
        reserved from other uses.

2021-02-09  Stuart Maclean

        * Release 8.0.5

        - Enter EM1 (low energy CPU mode) in RTOS Idler thread.

2020-12-04  Stuart Maclean

        * Release 8.0.4 (internal only, interim release)

        - Fix park phase timestamping. Blob ID was not being reset to 0x78,
        so would hit 0x7F limit after 8 dives and no further timestamps
        would be included in xmitted data.
        
        - Print to console Guest name if any Guest interface is compiled in.
        
        - Print to console program load offset (normally 0x1000).
        
        - Improve build Makefile, makes DEFINES easy to inspect.
        
        - Improved build process handling of simulated devices: ctd, iridium.

OTHER:

        Other/Earlier Tags, auto-derived with help from git for-each-ref, i.e.

        $ git for-each-ref --sort=-creatordate --format '%(refname) %(creatordate)' refs/tags 

refs/tags/10.4.6 Mon Nov 21 10:54:24 2022 -0800
refs/tags/10.4.5 Fri Oct 21 12:50:49 2022 -0700
refs/tags/10.4.4 Thu Sep 15 14:36:30 2022 -0700
refs/tags/10.4.3 Mon Sep 12 09:34:17 2022 -0700
refs/tags/10.4.2 Mon Sep 12 09:15:14 2022 -0700
refs/tags/10.4.0 Fri Jul 1 15:40:41 2022 -0700
refs/tags/10.2.3 Wed Jun 22 10:34:57 2022 -0700
refs/tags/10.3.0 Mon May 23 12:35:35 2022 -0700
refs/tags/10.2.2 Thu May 19 13:34:21 2022 -0700
refs/tags/10.2.1 Thu May 19 11:46:03 2022 -0700
refs/tags/v10.2.2 Wed May 18 15:19:11 2022 -0700
refs/tags/v10.3.0-rc1-mm2 Tue Apr 5 11:42:54 2022 -0700
refs/tags/v10.2.1 Tue Apr 5 11:42:54 2022 -0700
refs/tags/10.2.0 Fri Jan 21 15:13:46 2022 -0800
refs/tags/v10.2.0 Fri Jan 7 16:09:22 2022 -0800
refs/tags/10.0.1 Thu Dec 2 15:31:02 2021 -0800
refs/tags/v10.0.1 Thu Dec 2 10:58:50 2021 -0800
refs/tags/10.0.0 Tue Nov 23 14:20:39 2021 -0800
refs/tags/v10.0.0 Sun Nov 21 10:35:10 2021 -0800
refs/tags/v10.0.0-rc7-rbr4odo Thu Oct 21 15:16:04 2021 -0700
refs/tags/v10.0.0-rc7-rbr4 Thu Oct 21 15:16:04 2021 -0700
refs/tags/v10.0.0-rc6-rbr4 Thu Oct 21 15:16:04 2021 -0700
refs/tags/v10.0.0-rc5-rbrodo Wed Oct 20 11:11:13 2021 -0700
refs/tags/v10.0.0-rc5-rbr4odo Wed Oct 20 11:11:13 2021 -0700
refs/tags/8.0.5 Wed Feb 10 10:07:26 2021 -0800
refs/tags/v8.0.5 Mon Nov 16 16:23:21 2020 -0800
refs/tags/8.0.3 Mon Nov 16 16:23:21 2020 -0800
refs/tags/8.0.2 Wed Sep 2 12:29:11 2020 -0700
refs/tags/8.0.1 Wed Aug 5 12:16:52 2020 -0700
refs/tags/8.0.0-RBR Thu Jun 25 08:01:32 2020 -0700
refs/tags/CHIRP Tue Jun 23 12:34:21 2020 -0700
refs/tags/8.0 Mon Jun 8 12:13:42 2020 -0700
refs/tags/WIMS Fri May 29 07:59:04 2020 -0700
refs/tags/8.0.0 Wed May 27 07:20:30 2020 -0700
refs/tags/7.3.5 Thu May 7 18:56:30 2020 -0700
refs/tags/7.3.4 Wed May 6 07:12:40 2020 -0700
refs/tags/7.3.3 Tue May 5 14:43:14 2020 -0700
refs/tags/7.3.2 Tue May 5 08:42:01 2020 -0700
refs/tags/7.3.1 Wed Apr 29 13:31:33 2020 -0700
refs/tags/7.3.0 Fri Apr 24 11:47:08 2020 -0700
refs/tags/7.2.9 Thu Apr 2 14:58:39 2020 -0700
refs/tags/7.2.8 Thu Mar 26 11:40:37 2020 -0700
refs/tags/7.2.7 Wed Mar 18 16:12:14 2020 -0700
refs/tags/7.2.6 Wed Mar 18 11:53:51 2020 -0700
refs/tags/7.2.5 Wed Mar 18 08:51:20 2020 -0700
refs/tags/7.2.4 Fri Mar 13 15:29:38 2020 -0700
refs/tags/7.2.3 Tue Mar 10 13:30:18 2020 -0700
refs/tags/7.2.6-chirp Tue Mar 10 13:13:38 2020 -0700
refs/tags/7.2.5-chirp Mon Mar 9 11:13:43 2020 -0700
refs/tags/7.2.4-chirp Mon Mar 9 08:15:54 2020 -0700
refs/tags/7.2.3-chirp Fri Mar 6 14:48:56 2020 -0800
refs/tags/7.2.2 Wed Mar 4 11:45:37 2020 -0800
refs/tags/7.2.1 Tue Mar 3 08:00:10 2020 -0800
refs/tags/7.2.0 Mon Mar 2 13:34:01 2020 -0800
refs/tags/7.1.16 Wed Feb 19 14:13:33 2020 -0800
refs/tags/7.1.13-dev Wed Feb 19 14:13:11 2020 -0800
refs/tags/7.1.15 Tue Feb 18 13:01:42 2020 -0800
refs/tags/7.1.14 Tue Feb 18 07:43:06 2020 -0800
refs/tags/7.1.13 Fri Feb 14 09:25:59 2020 -0800
refs/tags/7.1.12 Mon Dec 9 12:50:06 2019 -0800
refs/tags/7.1.11 Tue Nov 26 12:30:13 2019 -0800
refs/tags/7.1.10 Thu Nov 21 11:52:06 2019 -0500
refs/tags/7.1.9 Wed Nov 20 20:02:43 2019 -0500
refs/tags/7.1.8 Wed Nov 20 19:46:51 2019 -0500
refs/tags/7.1.7 Wed Nov 20 18:11:14 2019 -0500
refs/tags/7.1.6-Host-Guest Wed Nov 20 14:04:59 2019 -0800
refs/tags/7.1.5-Host-Guest Tue Nov 19 13:50:11 2019 -0800
refs/tags/7.1.4-Host-Guest Tue Nov 19 06:20:49 2019 -0800
refs/tags/7.1.3-Host-Guest Mon Nov 18 07:44:57 2019 -0800
refs/tags/7.1.0-Host-Guest Fri Nov 15 16:57:21 2019 -0800
refs/tags/7.0.0-Host-Guest Tue Oct 22 15:59:03 2019 -0700
refs/tags/6.5.w-sbe Wed Oct 9 07:32:02 2019 -0700
refs/tags/wims-SBE-update Tue Sep 17 08:49:03 2019 -0700
refs/tags/6.8.5 Tue Aug 20 11:00:18 2019 -0700
refs/tags/6.8.4 Wed Jul 31 12:44:27 2019 -0700
refs/tags/6.8.3 Mon Jul 22 14:29:46 2019 -0700
refs/tags/6.8.2 Tue Jul 16 07:45:30 2019 -0700
refs/tags/6.7.0 Tue Jun 25 13:44:57 2019 -0700
refs/tags/6.6.0 Tue Jun 25 13:39:08 2019 -0700
refs/tags/6.5.5 Wed Jun 12 10:48:53 2019 -0700
refs/tags/6.5.4-XC Tue Jun 11 13:59:59 2019 -0700
refs/tags/6.5.4 Tue Jun 11 12:42:28 2019 -0700
refs/tags/6.5.w Mon Jun 3 08:24:13 2019 -0700
refs/tags/6.5.1 Fri May 24 14:28:54 2019 -0700
refs/tags/6.5.0 Wed May 22 15:10:14 2019 -0700
refs/tags/6.4.12 Tue May 21 09:29:21 2019 -0700
refs/tags/6.4.9 Fri May 3 11:14:32 2019 -0700
refs/tags/6.4.6 Fri Apr 26 08:56:20 2019 -0700
refs/tags/6.4.5 Thu Apr 25 10:58:38 2019 -0700
refs/tags/6.4.4 Wed Apr 24 15:18:42 2019 -0700
refs/tags/rm Fri Apr 12 12:06:45 2019 -0700
refs/tags/6.4.0 Fri Apr 12 12:06:45 2019 -0700
refs/tags/6.2.5 Wed Feb 27 15:47:10 2019 -0800
refs/tags/6.2.4 Tue Feb 19 12:24:09 2019 -0800
refs/tags/6.1.3 Tue Jan 15 14:26:59 2019 -0800
refs/tags/6.1.2 Fri Dec 21 08:02:12 2018 -0800
refs/tags/6.1.1 Thu Dec 20 16:05:57 2018 -0800
refs/tags/6.1.0 Fri Nov 30 14:23:23 2018 -0800
refs/tags/6.0.3 Wed Nov 28 15:12:22 2018 -0800
refs/tags/6.0.2 Tue Nov 27 07:07:08 2018 -0800
refs/tags/6.0.1 Tue Nov 6 07:37:58 2018 -0800
refs/tags/6.0.0 Fri Aug 17 22:55:40 2018 +0000
refs/tags/5.1.b Tue Apr 24 18:11:09 2018 +0000
refs/tags/5.0.a Thu Jun 8 11:04:15 2017 +0000
refs/tags/4.6.a Wed May 17 19:46:33 2017 +0000
refs/tags/4.5.d Wed Apr 26 19:11:31 2017 +0000
refs/tags/4.5.c Fri Apr 21 19:02:20 2017 +0000
refs/tags/4.5.a Fri Mar 10 14:04:24 2017 +0000
refs/tags/4.4.a Sat Feb 25 16:55:36 2017 +0000
refs/tags/4.3.a Fri Feb 3 23:25:17 2017 +0000
refs/tags/4.2.b Sun Jan 15 06:39:34 2017 +0000
refs/tags/4.1.a Sat Dec 24 12:05:38 2016 +0000
refs/tags/4.0.a Sat Oct 1 11:26:27 2016 +0000
refs/tags/3.2.a Sat Aug 20 23:52:28 2016 +0000
refs/tags/3.1.g Mon Jul 18 03:44:00 2016 +0000
refs/tags/3.0.g Mon Jul 18 03:42:12 2016 +0000
refs/tags/3.0.e Sat May 14 22:18:36 2016 +0000
refs/tags/3.0.a Sun May 1 03:24:22 2016 +0000
refs/tags/2.3.e Tue Apr 12 04:12:58 2016 +0000
refs/tags/2.3.d Mon Apr 4 07:39:04 2016 +0000
refs/tags/2.3.c Sun Mar 20 07:46:32 2016 +0000
refs/tags/2.3.b Wed Mar 2 17:31:19 2016 +0000
refs/tags/2.4.a Wed Feb 24 15:11:37 2016 +0000
refs/tags/2.3.a-1 Tue Feb 16 20:13:44 2016 +0000
refs/tags/2.3.a Sun Jan 24 00:50:26 2016 +0000
refs/tags/2.2.a Tue Nov 17 19:01:28 2015 +0000
refs/tags/2.1.a Mon Sep 28 09:49:27 2015 +0000
refs/tags/2.0.i Wed Aug 26 12:06:02 2015 +0000
refs/tags/2.0.h Tue Aug 18 18:33:37 2015 +0000
refs/tags/2.0.g Fri Aug 14 18:17:29 2015 +0000
refs/tags/2.0.e Tue Jul 28 07:55:24 2015 +0000
refs/tags/2.0.d Wed Jul 22 12:00:15 2015 +0000
refs/tags/2.0.c Mon Jun 29 10:09:34 2015 +0000
refs/tags/2.0.b Mon Jun 1 22:01:42 2015 +0000
refs/tags/2.0.a Thu Feb 12 19:36:16 2015 +0000
refs/tags/1.9.b Fri Jan 16 19:52:27 2015 +0000
refs/tags/1.9.a Wed Jan 7 23:46:02 2015 +0000
refs/tags/1.8.d Tue Sep 2 15:28:07 2014 +0000
refs/tags/1.8.c Tue Aug 12 16:06:33 2014 +0000
refs/tags/1.8.b Tue Aug 5 15:07:25 2014 +0000
refs/tags/1.8.a Sun Jul 20 23:25:41 2014 +0000
refs/tags/1.7.b Sat Jul 19 19:25:31 2014 +0000
refs/tags/1.7.a Thu Jul 17 20:43:32 2014 +0000
refs/tags/1.6.c Mon Jul 14 23:35:47 2014 +0000
refs/tags/1.6.b Sat Jul 12 19:33:00 2014 +0000
refs/tags/1.6.a Wed May 21 22:04:45 2014 +0000
refs/tags/1.5.a Mon May 5 20:54:37 2014 +0000
refs/tags/1.4.b Tue Apr 29 21:28:28 2014 +0000
refs/tags/1.3.a Tue Apr 22 05:12:28 2014 +0000
refs/tags/1.2.a Wed Mar 12 10:52:34 2014 +0000
refs/tags/1.1.a Fri Mar 7 12:44:10 2014 +0000
refs/tags/1.0.a Thu Oct 24 23:19:12 2013 +0000
refs/tags/0.5.a Tue Aug 6 19:10:11 2013 +0000
```

## VOP firmware 8.0.0 – 8.0.3

```text
        
        * v8.0.3 (from history)

        - Packing fixes NN offset by 16 and rbr-surface passes valid addresses 
        internally. This fix allows for the upload of high resolution data 
        where wrapping behavior was discovered in the ascent data. 

2020-09-02 Christian Sarason 

        * v8.0.2 (from history)

        - Sync interface implementation so that all RBR measurement requests go through the same API

2020-08-05 Christian Sarason

        * v8.0.1 (from history)

        - Revise [rbr-surface](dependencies/rbr-surface) statemachine used 
        in the [surface](source/application/surface.c) state to treat the 
        RBR sensor as a streaming device as opposed to a line-terminated 
        response device.

        - Also ensured that timeout conditions in the statemachine 
        synced curvature-packed data to disk to send over Iridium.

2020-05-27 Christian Sarason

        * v8.0.0 (from history)

        - Add WIMS to the surface state.

        - Using product identifier in the last 8 bits of the *version* field in the `0xD0` (mission parameters) blob.

        - WIMS = product 1 (slots 1-50 allocated for WHOI special projects).

        - CHIRP = product 51 (slots 51-100 allocated for NUWC special projects).
```

## MFA firmware 1.5.a – 4.5.d

```text
===Version 4.5.d - VOP_SSSS_3051_v45d_x.bin - Apr 2017 ===
- clear PTD once used during park state.
- allow PTD value to be set to 0.


===Version 4.5.c - VOP_SSSS_3048_v45c_x.bin - Apr 2017 ===
- fixed raw data mode for Seabird sensor.
- added error for park date/time (PTD) set in the past.


===Version 4.5.a - VOP_SSSS_3047_v45a_x.bin - Apr 2017 ===
- fixed empty bin processing for Seabird sensor.
- added date/time configuration parameter (PTD) for exiting park state. PTD is converted into seconds after it is
processed and set to zero after one time use.
- added motion sensor sampling during park state.
- added MAT (two parameters) configuration parameter for motion sensor sampling intervals and timeout.
- added MAP configuration parameter for motion sensor sampling period.
- added PTD, MAT1, MAT2, MAP configuration parameters into mission configuration packet (0xDn).


===Version 4.4.a - VOP_SSSS_3031_v44a_x.bin - Mar 2017 ===
- added processing and transmission of extra temperature channel for RBR CTD sensors.
- modified scaled (0x2n) and raw (hi-res, 0xAn) temperature data packets to incorporate sensor channel for distinguishing
extra temperature channel data.
- added CTC configuration parameter for enabling extra temperature channel data.
- modified surface engineering packet (0xEA) to include sensor profile data values.


===Version 4.3.a - VOP_SSSS_3024_v43a_x.bin - Feb 2017 ===
- modified ascend rise rate calculation with ARC parameter.
- modified DFT pump time adjustment from seek, value not changed if valve operation does not change depth.
- added seek valve configuration parameters DVD and DVW and incorporated into mission configuration packet (0xDn).
- removed redundant timestamps from mission configuration packet and added sensor type and packet version (0xDn).
- added empty bin count to surface engineering packet (0xEA).
- modified descend and park seek delay times to static values for finer control in sampling loops.
- implemented raw data mode to return specified number of samples starting at a specified depth for Seabird sensor.
- added CRD and CRS configuration parameters for raw data mode.
- added loop count to surface data packetization to prevent cases sensor data retrieval issues.
- updated assert errors, included surface packetization loop count exceeded.


===Version 4.1.a - VOP_SSSS_3008_v41a_x.bin - Jan 2017 ===
- fixed issue with Seabird profile data empty bin processing causing different profile data indexing with p,t,s.
- updated valve driver for new open/close calibration calculation.
- added check for valid depth value for depth trigger check.
- modified the number of rise and fall pressure storage values to 600.
- modified command echo packet to return all errors in field if command packet not processed correctly.
- modified command echo packet (0xDE) processing to send response for error packets as well.
- added command (MTC) to exit check for trigger pressure.
- fixed issue with valve packet generating extra packet data.
- modified data storage clear functions in states to check for null pointer before clearing.


===Version 4.0.a - VOP_SSSS_2999_v40a_x.bin - Oct 2016 ===
- fixed calculation of the number of samples available to store in states for rise/fall/park packets.
- modified banner for MRV Firmware Architecture (MFA), not ALAMO specific.
- RBR PAR sensor channel added to driver.
- added new sensor data packet (0xC2) to handle transmission of PAR data.
- added gain (SGS) and offset (SOS) parameters for PAR sensor data.


===Version 3.2.a - VOP_SSSS_2996_v32a_x.bin - Sept 2016 ===
- modified default satellite timeout configuration parameter from 5 to 15 minutes.
- added configuration parameter transmission to built-in test.
- ice avoidance algorithm modification to skip surface state if pump time is exhausted (APM) and depth is greater than ICX, outside the temperature calculation max depth. Accounting for case where float is possibly stuck under the ice shelf and is unable to reach the surface.
- bluetooth mode (BTM) parameter, also in the mission configuration packet (0xD0), determines if the Bluetooth is enabled/disabled after the BIT completes.


===Version 3.1.g - VOP_SSSS_2983_v31g_x.bin - July 2016 ===
- used for S2-H HYBRID.
- modified seek function to allow control of initial pump time.
- removed initial pump time adjustment from ice seek algorithm.
- updated maximum depth (MDS) command parameter for hybrid operation.
- added ascend flag for determining if profile data packetized during ice algorithm to prevent resending same data during surface state.
- modified seek to allow parameter to determine if descend pump time is adjusted.
- removed ice algorithm seek descend pump time adjustment.
- moved storage of minimum depth in ice algorithm to store each time through ascend state loop.
- fixed BIST type check.
- modified defaults for ice algorithm surface search timeout (10 to 3 minutes); hold valve timeout (0 to 1 second).
- added ISM parameter to mission configuration parameters packet.
- modified rise/fall packet storage to have single points for ice turnaround and seek state codes.


===Version 3.0.e - VOP_SSSS_2973_v30e_x.bin - May 2016 ===
- modified ice seek.
- added parameters for ice seek, modified x-message for parameters.
- added ice seek parameter to engineering data.
- modified park to exit during seek wait.


===Version 3.0.a (VOP_X_2971_v30a_xXXX.bin) - May 03 2016 13:56 ===
- incorporated ice avoidance algorithm.
- optimized valve control in descend from fall function.
- optimized pump control in seek depth function.
- added separate timeout macro for fall from depth in the park state.


===Version 2.3.b (VOP_RBR_PT_2963_v23b_xE32F.bin) - Mar 04 2016 13:18:32 ===
- modified SBE sensor profile start command to include delay for pressure sampling issue.
- fixed issue with empty bins for profile data with Seabird sensor driver.
- fixed x-message time conversion issue for leap years.
- debug output modifications.
- fixed issue with rise/fall, surface, and park packets from modified CTD data structure type.
- fixed rise/fall packets spanning issue.
- added check for minimum number of samples before bin averaging in Seabird sensor driver.
- updated output banner.


===Version 2.3.a (VOP_RBR_PT_2952_v23a_x7538.bin) - Feb 18 2016 12:46:29 ===
- lengthened CTD sensor power up delay to accommodate sensor hardware changes.
- modified built-in test to accommodate long (lab) test and short (deployment) test. Need deployment pass/fail indicator.
- modified rise/fall code states for x-message/Argo compliance.
- added gyro and accel scale values to surface motion packet.
- added surface pump timeout parameter to run pump at surface for buoyancy before air assist.
- modified version in mission configuration parameters packet to use compiled values rather than EEPROM.
- separated pump stop and data store operation functions.
- implemented Seabird CTD sensor (41CP+) driver and incorporated modifications for application layer support.
- fixed issue with mic 99 command during float operation stopping mission.
- fixed issue with configuring CTD sensor with proper (stored) configuration parameters.
- increased the pump packet (0x60) cycle storage limit from 100 to 200.
- added fields for reset of surface pressure offset in BIT packet.
- added fields for reset of surface pressure offset in surface engineering packet.
- added gps/rtc time delta to surface engineering packet.
- modified rise/fall packets to reset start time for each new packet index.
- fixed pump data storage issue for different mission states.
- added valve engineering packet.
- added function to add back in valve open time to allow for more pumping time.
- fixed accel, gyro gain defaults and accel sample rate default.
- removed previous mission and after reset pressure fields from surface engineering data packet.
- added additional sensor configuration output messages.


===Version 2.0.h (VOP_CTD_2916_v20h_xA68C.bin) - Aug 18 2015 12:21:01 ===
- updated software revision.
- fixed issue with signed/unsigned comparison in descend seek algorithm.
- included salinity (CTD) modifications, controlled by compile-time macro (CTD_SALINITY).
- fixed salinity packet number reset.
- added salinity park packet.
- fixed park engineering data salinity average value.
- fixed air pump zero values in surface engineering packet.
- fixed timeout for salinity data download from CTD (larger timeout needed to get more data).
- clean up for curvature packing fix.
- fixed curvature packing.
- added transmission of Argo packet to BIT.
- modified parameter restore during surface state to prevent overwriting start times and storing diagnostic dive parameters.
- fixed issue with curvature packing sizes 1 and 2 samples left for packing.
- fixed GPS SVINFO packet parsing for SNR field.
- modified BIT mission number (-1).
- removed fuse test descend parameter output.
- updated descend engineering x-message version number.
- modified air pump failure check, removed lower limit check.
- fixed pump packet count.
- fixed pump time for descend seek.
- removed CTD storage from air assist loop.
- moved packetization in surface before loading configuration parameters to fix timestamp issue on diagnostic dive.
- fixed BIT packet.
- fixed pump current get function.
- updated pump packet data.
- updated engineering packet data.
- updated configuration parameter packet.
- updated built-in test.
- added minimum/maximum case pressure and battery voltage parameters.
- added calibration parameter values.
- added functions for getting case pressure and battery voltage.
- powered down CTD and RTC for low power mode.
- enabled accelerometer get functions for sampling rate and scaling.
- updated air and oil pump test functions.
- updated initial banner.
- added air and oil pump test functions.
- added ADC calibration commands.
- added ADC command to retrieve calibration values.
- added oil pump test command.
- added ADC calibration get function.
- added/modified park seek default values.
- updated descend parameter dump variable names for seek and calibration algorithm.
- updated pump and valve minimum macros.
- updated default parameters for descend state.
- included maximum depth check in seek algorithm loop.
- added comments with command numbers for x-message configuration parameters packet.
- added pump information storage for pump data packet during seek operations.
- checked diagnostic function for new seek algorithm.
- updated mission configuration parameters x-message packet to incorporate new descend and park seek algorithm.
- added park seek operation.
- added park seek configuration parameters and commands.
- removed debug print out from curvature packing.
- added parameter for target depth for descend seek calculation function making it more generic.
- updated descend seek function to take the depth measurement time as a parameter to allow for more generic calculation in seek.
- updated pump time adjustment variable storage.
- updated descend seek algorithm.
- modified GPS command buffer for larger command responses.
- interim check in for seek descent algorithm.
- updated GPS buffer size to accommodate more satellite channels for commands.
- moved surface data collection after GPS fix is obtained.
- added macros for packing format field.
- updated surface data packet to move version field into length similar to rise/fall 
- fixed packet number incrementing for park packet types.
- modified range of time delay to prevent divide by zero in descend seek algorithm.
- added compression command setting output to mission parameters.
- fixed issue with surface salinity initialization.
- remove high nibble format field from length field for surface packet types.
- updated Argo packet and support functions.
- updated packet size check in x-message packetization functions.
- modified data management to accommodate new surface packet.
- removed surface data version field.
- fixed fall, rise, and pump length and code fields.
- renamed difference packing function.
- fixed issue with difference packing data size return parameter.
- fixed pump data size issue for packetizing.
- updated CTD sensor maximum sample buffer size for curvature packing algorithm.
- modified curvature packing algorithm.
- fixed issue in CLI serial output limiting the size of output strings.
- interim checkin for curvature packing testing.
- modified x-message packing function parameters to accommodate curvature compression algorithm.
- modified surface data packetize function for curvature packing algorithm.
- added descend algorithm parameter.
- interim checkin for curvature data packing algorithm.
- updated surface motion data collection.
- modified BIST and packet to include CTD measurement.
- added parameter for motion data display command for raw or scientific data mode.
- modified the BIST mission number.
- added gain and offset configuration parameters and commands.


===Version 1.8.c (VOP_2287_18c_xCCDF.bin) - Aug 28 2014 14:53:50 ===
- modified valve assert errors to differentiate open close errors.
- added halt rise rate and halt depth delta configuration parameters and commands. Used in the
  modified ascend halt condition after maximum pump time is reached.
- modified ascend halt algorithm after maximum pump time reached to check if near the surface to 
  allow for transition to surface state.
- added pump data storage function in ascend state for other states to include info in pump data packet.
- added code to extend the GPS timeout to more closely wait the time specified by the user.
- modified the serial passthrough mode for CTD testing.
- added coefficient commands in CTD sensor and display at boot time.
- fixed issue with multiple zero packet IDs.
- modified descend state to store pump info in pump data packet.
- set default park sampling interval to 10 minutes.
- added CTD sample to start of each state.
- set default GPS timeout to 15 minutes.
- fixed pump packet voltage to 0.01V value instead of mV.


===Version 1.8.b (VOP_2277_18b_xAEEF.bin) - Aug 8 2014 15:38:54 ===
- moved CTD sensor disable after profiling to the surface state.
- set default profiling mode to bin averaging.
- modified start depth to 100 dbar and start timeout to 3 hours.
- fixed packet count in surface engineering packet.
- modified when configuration parameters are loaded during surface state to account for case
  where configuration parameters are modified ensuring proper parameters are stored and used.
- reset the maximum air pressure variable during the surface state.
- modified initial depth to 50 dbar.
- added case vacuum to descend pump data and ascend pump data.
- added default timeout after calibration during descend to allow for fall time to configured depth.
- added pump voltage to descend engineering data.
- added pump voltage to pump data packets.


===Version 1.8.a (VOP_2263_18a_x9AF5.bin) - Aug 4 2014 15:21:17===
- added get start depth threshold in ascend state for beacon mode operation.
- added exit beacon mode in surface state if depth is lower than descend start depth threshold.
- set maximum values for maximum and descend target depths.
- fixed issue with packet index for scaled and hires pressure and temperature data.
- fixed sending zero data packet for pressure and temperature data.
- fixed zero sample and the end of fall, rise, and surface packets.
- added remote command echo packet.
- added diagnostic dive start timeout and depth.
- added check in park if at surface, transition to next state and do not execute fall.
- added return of RTC for GPS data if no time fix is obtained.
- modified default values for CTD bin average mode.
- set GPS position to invalid fix values when no fix is returned.
- park state sample mode default set to raw to collect all data.
- put code back in to wait forever in ascend state if pumped maximum time and not at surface.
- initial modifications for remote command echo.
- fixed transmission attempts count in surface engineering packet.
- fixed current packets to transmit in surface engineering packet.
- fixed packet count restart for each transmission.
- added reset CTD memory after data collected and packetized at surface.
- added CTD samples before GPS, after GPS, before satellite transmit.
- fixed time conversion in x-message for zero values.
- fixed pump data packet.
- fixed minimum GPS signal level.


===Version 1.7.b (VOP_2249_17b.bin)===
- fixed issue with compression algorithm for (10,20,90,A0 packets) where an incorrect pointer was used
  for a buffer size parameter.
- added park override of parameters for diagnostic dive.


===Version 1.7.a (VOP_2245_17a.bin)===
- modified code to ensure whole data fields are packetized in each x-message packets.
- fixed issue with incorrect transmit packet count in surface engineering data. The index was
  previously used to determine packet count, however, the next packet could possibly fit into a previous FIFO.
- added version field to BIT x-message packet.
- removed redundant buffer clear.
- fixed issue with GPS time to fix, need to divide value by 10 for x message format.
- added temperature data packets to park state for raw mode.
- fixed issue with park pressure and temperature packets (0x98-F,0xA8-F) not separating data in buffer correctly.
- reset descend CTD variable for fall fuse testing.
- reset park CTD variable for fall measurements.


===Version 1.6.c (VOP_2236_16c.bin)===
- updated surface engineering packet.
- corrected GPS command fix location output.
- fixed GPS packet, corrected longitude fix type to allow for signed values.
- fixed GPS packet data fields.
- fixed issue with mission configuration parameter x message packetization.
- added external ADC driver.
- fixed issue with size of mission configuration packet.


===Version 1.6.b (VOP_2231_16b.bin)===
- added clear CTD memory during initialization.
- fixed calibration rate calculation for accounting for return to surface scenario.
- updated pump rate value in mission configuration packet.
- implemented new calibration algorithm.
- implemented fall time adjustment based on new descend target depth.
- fixed rise/fall packet having start time in each packet.
- set default on pump time to 10 seconds during ascend state.
- fixed descend rate calculation for calibration.
- moved state structures to header files for mission configuration packetization.
- added ascend rise rate adjustment factor parameter for calculation for more accurate rise rate.
- removed pump forever code from ascend state.
- modified descend calibration algorithm to adjust fall pump time for accurate target depth.
- modified default descend target and park depths.
- added version fields to engineering data packets.
- added mission configuration packet function.
- fixed motion data packetization and header construction, moved into data management.
- fixed motion data packetize x message function using incorrect pointer for first byte.
- fixed mission configuration x message function using incorrect pointer for first byte.
- fixed CTD sensor driver setting error when status was ok.
- updated accelerometer mode command for bit mask values and upper range limit.
- added mission configuration function.
- fixed descend from depth valve open calculation rounding up.
- fixed pump data vacuum fields sizes.
- perform diagnostic dive for mission command set via CLI as well as deployment mode.


===Version 1.6.a (VOP_2210a_16a.bin)===
- fixed issue with EEPROM address for calculating checksum after moving configuration data address.
- updated GPS driver allowing for larger command response sizes. 
- implemented GPS info fields by adding additional commands to driver.
- updated to support additional GPS x-message packet fields.
- updated accelerometer commands, added FIFO data retrieval command. 
- updated accelerometer (MPU) driver to support FIFO data sample collection. 
- updated surface state motion data collection to use accelerometer FIFO.
- added ACF command for accelerometer FIFO sampling.
- moved system serial number out of mission state parameters to fix clearing value when EEPROM is reinitialized with default values from new build.
- moved configuration parameters start address in EEPROM (device 0).
- added BIT GPS packet to support version 2.1 x-message format. 
- added diagnostic dive support and configuration parameters. 
- modified mission number operation for diagnostic mission. 
- modified Argo packet data according to version 2.1 x-message format.
- added code to ensure only 8 packets transmitted for park and hires data. 
- changed default parameters for park state.
- fixed issue with satellite driver for parsing received messages.
- added ascend state engineering packetize function. 
- modified code to use macros for ADC conversions. 
- removed unnecessary state parameter members, configuration parameters. 
- implemented initial x-message version 2.1 support software.
- updated state engineering packets. 
- extended functionality of surface engineering packet to include various ADC values. 
- extended functionality of BIT (include valve test, satellite device comm test).
- removed unused ascend, park, surface target depth mission configuration parameters. 
- modified park target depth parameter name. 
- added data management functions to get packet count and transmission attempts count.
- added surface engineering packetize function, not using general engineering packetize function.
- modified surface engineering packet (added packet count and transmission attempts fields).
- decreased error log storage size for surface engineering packet.
- replaced GPS structure values for x-message packets.
- modified default ascend surface threshold from 1 dbar to 2 dbar. 
- added park hour timeout configuration parameter. 
- added park hour timeout code to determine next park exit based on hour interval. 
- modified default surface gps timeout from 5 to 8 minutes.
- added park engineering packetize function for x-message format.
- moved state engineering data structures to header files for use in x-message packetization functions.
- added auto-transmit of mission configuration parameters with configuration parameter.
- added descend function call from park and valve timeout for descend from park.
- added parameter to accelerometer get data to display values to console.
- added hires packet types for raw data.
- added park pressure and temperature packet types (similar to rise/fall data).
- added code to allow for other packet types to increment lower nibble of sensor id field.
- removed fuse test buffer, store data in descend ctd buffer.
- moved fuse test code to separate function to allow calling from other states.
- changed descend engineering packet data storing (removing fuse timestamps from structure).
- added descend rate parameter.
- added park engineering data.
- moved pressure and temperature averaging function from xmsg to park.
- added support for different gps packets from surface.
- updated park engineering packetize function.
- removed ctd sensor extra debug output.
- filled in time_for_fix gps field.
- modified packet ID field in GPS packet rather than static value. 
- fixed issue with struct tm month field 
- added get CTD status to sensor driver.
- added start times to state engineering data.
- added sensor status get to BIT.
- added sensor status get command.
- added software revision to the mission configuration.
- added CTD status and ADC values (case vac and vbat) to surface engineering message.
- modified ascend engineering data to send as a pump series packet ID to allow for spanning multiple packets. 
- modified data management to allow for spanning multiple packets by incrementing the packet count in the least significant nibble of the sensor ID field.
- modified park sample mode to allow for default of average CTD data and raw mode. 
- added park CTD data packetizing. Engineering data contains the averaged CTD data. 
- modified accelerometer flag to a bit mask to allow for storing accelerometer data in multiple states during a mission.
- added accelerometer reset to configuration function and checking of data ready for magnetometer data reading.
- code cleanup, removing unused function in CTD driver.
- clean up ascend and surface state functions. 
- added assert for CTD data get retry.
- clean up state parameters removing unnecessary values.
- added park fall configuration parameters. 
- fixed descend calibration calculation.
- added accelerometer sampling during surface state.
- added accelerometer data packetization functionality.
- set default descend sample interval to 1000 ms.
- removed descend state parameter surface depth threshold.
- added pump current to descend engineering packet.
- added additional parameters to regimes command in rbr.c as per RBR suggestion.
- added accelerometer related command support.
- fixed issue with fuse timeout command setting.


===Version 1.5.a (VOP_2152_15a.bin)===
- modified console check during mission control task.
- added initial trigger depth and timeout configuration parameter and command.
- updated park packet averaging and packetizing function.
- fixed issue with closing UART port for satellite and GPS communications. The UART ports for the satellite and GPS should be left open, since they are opened during initialization.
- added accelerometer mode parameter and command for surface sampling data.
- adjusted surface halt condition parameters (wait for 2 hours, sample every 30 seconds).
- fixed issue with running valve calibration during BSP initialization rather than in BIT.
- update ascend debug message.
- increase ARR upper limit.
- added park timeout configuration command.
- moved BIT to mission control task.
- moved CTD configuration to mission control initialization.
- remove minute delay between mission dive cycles.
- added checksum to configuration parameter data storage.
- updated ascend pump data engineering data.
- added pump current get function to driver.
- updated ascend pump timestamp buffer size. Need to change this to accommodate pump on depth.
- added monitoring for depth when console not connected in mission control task.
- added exit beacon mode in surface state if beacon off command is sent.
- moved GPS UART open to BSP configuration. UART is left open and never closed.
- removed unused GPS functions.
- modified maximum pump time to 17 minutes.
- removed default parameter setting in GPS timeouts.
- added pump on/off timestamping in ascend state. Modified ascend engineering packet for pump on/off timestamp.
- added code in ascend state to only enable sensor profiling if depth is greater than the surface depth threshold.
- added failure code in ascend state to turn on pump after set timeout (10 hours) if all pump intervals have failed to cause passing the surface depth threshold. Pump is then run forever or until passing surface depth threshold.
- updated BIT to perform GPS, packetize, and send results.
- added default fuse test parameter values.
- modified serial number get/set to use EEPROM.
- modified valve config to return status for BIT.
- fixed issue with maximum fuse tests in descend state.
- updated default maximum depth.
- added code to break out of park state if exceeded maximum depth.
- modified mission parameter variable name passed into state functions.
- added calibration code to descend state function and associated commands.
- moved state local variables out of state structure.
- implemented park state.
- added park state commands.
- added data compression set command to accommodate various compression schemes.
- added max pkt count if LIFO full & pkt count increment if LIFO not full
- added fuse test attempts configuration parameter and command.
- updated mission configuration store function parameter type.
- fixed initialization of park parameters.
- added rise rate debug output.
- added code to recalc #pkts to send after CLI process. "MIC 2" appears to work.
- patched data_mgmt_delete_top_pkt() to decrement filled_num before check to prevent lifoHead_idx wrapping to top on LIFO index0 deletion.
- fixed LIFO error when LIFO empty
- updated satellite transmit to clear modem buffer.
- updated revision number for next release.
- removed CTD data from state parameter structures to avoid sending in configuration command retrieve.
- removed GPS data from surface state parameter structure.
- implemented data compression algorithm select command.
- remove air assist mode from descend state.
- implemented new rise rate during ascend state.
- modified ascend default maximum pump time to set for maximum pump time in case cannot move from target depth.
- modified default surface sampling interval.
- added mission number to surface engineering packet data in case transmission happens during different mission.
- corrected issue with executing last command in remote command buffer.
- modified the remote command execution loop in xmsg_pkt_Receive_Command_Process function.
- added rise rate and command to ascend state.
- modified data management for changes to receive and process satellite messages.
- fixed issue in states storing first depth before start time causing fall/rise time calculation error.
- move receive X message parsing to xmsg_pkt file.
```
