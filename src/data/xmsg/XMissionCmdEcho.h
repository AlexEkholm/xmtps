/** The @ref util::xmsg::XMissionCmdEcho class source file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 1, 2014 2:13:38 PM
 *
 *  @author     Mr. Alexander K. Ekholm <aekholm@whoi.edu>,
 *              Engineer I,
 *              Department of Physical Oceanography,
 *              Woods Hole Oceanographic Institution
 *
 *  @copyright  2014 Woods Hole Oceanographic Institution.
 *              Released under the GNU General Public License.
 *
 *  This is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 *  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details. You should have received a copy of the GNU General
 *  Public License along with this software. If not, please visit http://www.gnu.org/licenses/.
 */

#ifndef XMISSIONCMDECHO_H_
#define XMISSIONCMDECHO_H_

/* Includes */

#include "XMessage.h"

/* Definitions */

namespace util {
namespace xmsg {

/* Class Definitions */

/** Class representing a ground station command echo-back X-Message (0xDE).
 *
 *  After a command upload, the float echoes the command text it received: the uploaded commands,
 *  separated by ';' (uploads end with "MIC 1"), and 8-9 binary bytes, before or after the text. The blob's own ';' terminator is found from its size, since ';' also separates the
 *  commands.
 */
class XMissionCmdEcho:
        public XMessage {

public:

    /* Member Functions */

    /** The parameterized constructor.
     *
     *  @param[in]      dive        The dive or cycle number of this message.
     *  @param[in]      whoi        The unique WHOI identifier.
     *  @param[in]      data        The binary X-message data to decode.
     *
     *  @throw          util::log::MessageException if an decode error occurs.
     */
    XMissionCmdEcho(int16_t dive, uint16_t whoi, const vector<uint8_t>& data) :
                    XMessage(dive, whoi, data) {
        DecodeXMsgData(data); // Decode message.
    }

    /** The destructor. */
    ~XMissionCmdEcho() {
        /* Do nothing. */
    }

    /** Decode the ground station command echo-back X-Message from the binary message data.
     *
     *  @param[in]      data        The binary message data to decode.
     *
     *  @throw          util::log::MessageException if the message is invalid or an error occurs during
     *                  decoding.
     */
    void DecodeXMsgData(const vector<uint8_t>& data) {

        try {

            // Check message.
            const uint8_t* d = CheckXMsgData(data, XMSG_TYPE_ID_MISS_CMD_ECHO, -1);
            const size_t len = 0x0FFF & u16(d, 1);

            /* Split the text into commands at ';', dropping blank entries. Besides the text the
             * echo carries 8-9 binary bytes of undocumented meaning, either before the text (all
             * NUL in every echo seen) or after it (NULs, the last byte sometimes not); so leading
             * NULs are skipped and the text ends at the next NUL. Non-printable text bytes are
             * kept as \xNN. */
            commands_.clear();
            string cmd;
            size_t start = 3;
            while(start + 1 < len && d[start] == 0)
                ++start;
            for(size_t i = start; i + 1 < len && d[i] != 0; ++i) {
                const uint8_t c = d[i];
                if(c == ';') {
                    AddCommand(cmd);
                    cmd.clear();
                } else if(c >= 0x20 && c < 0x7F) {
                    cmd += (char) c;
                } else {
                    ostringstream os;
                    os << "\\x" << hex << setw(2) << setfill('0') << (int) c;
                    cmd += os.str();
                }
            }
            AddCommand(cmd);

        }CATCH_BLOCK("Failed to decode ground station command echo-back message!")
    }

    /** Generate a listing string representation of the ground station command echo-back message:
     *  one CMD line per echoed command.
     *
     *  @param[in]  eidx    The index of this echo-back message within the dive.
     *
     *  @return     A listing string representation of the ground station command echo-back message.
     */
    const string GenerateListingString(int eidx) const {
        ostringstream os;

        try {

            int cidx = 0;
            for(string cmd : commands_) {

                // List the parameter name (the text before the first space) in uppercase,
                // leaving \xNN escapes as they are.
                for(size_t i = 0; i < cmd.size() && cmd[i] != ' '; ++i)
                    if(cmd[i] == '\\')
                        i += 3;
                    else if(cmd[i] >= 'a' && cmd[i] <= 'z')
                        cmd[i] -= 'a' - 'A';

                os << format("CMD %5d %4d %4d %4d  %s") % id_instrument() % id_dive() % eidx
                        % cidx++ % cmd << endl;
            }

        }CATCH_BLOCK("Failed to generate listing string ground station command echo-back message!")

        return os.str();
    }

    /** Get the echoed commands, in the order the float received them.
     *
     *  @return     The echoed commands.
     */
    const vector<string>& commands() const {
        return commands_;
    }

private:

    /** Add a command to @ref commands_, trimmed of surrounding spaces; blank commands are skipped.
     *
     *  @param[in]  cmd     The command text.
     */
    void AddCommand(const string& cmd) {
        const size_t b = cmd.find_first_not_of(' ');
        if(b == string::npos)
            return;
        commands_.push_back(cmd.substr(b, cmd.find_last_not_of(' ') - b + 1));
    }

    /** The echoed commands, in the order the float received them. */
    vector<string> commands_;
};

} /* namespace xmsg */
} /* namespace util */
#endif /* XMISSIONCMDECHO_H_ */
