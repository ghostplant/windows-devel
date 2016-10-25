
package callback;

import comapi.CommonLib;

public class ApplyAsyncCallback implements AsyncCallback {

    public String fid, fnick;

    public ApplyAsyncCallback(String fid, String fnick) {
        this.fid = fid;
        this.fnick = fnick;
    }

    public void execute() {
        try {
            System.out.println(fid + "," + CommonLib.connThread.ct.groups.elementAt(0).toString() + "," + fnick);
            CommonLib.send("addfrd", new String[]{fid, CommonLib.connThread.ct.groups.elementAt(0).toString(), fnick});
            CommonLib.MessageBox("添加好友请求已发送。");
        } catch (Exception e) {
            
        }
    }
}
