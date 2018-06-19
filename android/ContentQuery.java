import java.io.File;
import java.util.List;
import java.util.ArrayList;
import java.lang.Integer;
import android.util.Log;
import android.content.Context;
import android.content.ContentResolver;
import android.content.ContentUris;
import android.content.Context;
import android.database.Cursor;
import android.net.Uri;
import android.provider.MediaStore;
import android.provider.MediaStore.Audio.Albums;
import android.provider.MediaStore.Audio.Media;
import android.provider.MediaStore.Files.FileColumns;

public final class UsbBrowserList {

    public static final String TAG = "UsbBrowserList";

    private final Context mContext;

    public UsbBrowserList(Context serviceContext) {
        mContext = serviceContext;
    }

    public List<FolderItem> getFolderList(int playPart, int folderId, int startIndex, int count) {
        Log.d(TAG, "getFolderList playPart = " + playPart + "folderId = " + folderId + "startIndex = "
                + startIndex + "count = " + count);
        List<FolderItem> list = new ArrayList<>();
        Cursor cs = null;

        try {
            cs = queryFolder(playPart);
            if (cs != null && cs.getCount() != 0) {
                if (!cs.moveToPosition(startIndex)) {
                    Log.e(TAG, "Cursor moveToPosition(" + startIndex + ") failed. getCount=" + cs.getCount());
                    return list;
                }
                do {
                    if (list.size() >= count) {
                        break;
                    }
                    int tItemIndex = startIndex;
                    int tFolderParentId = -1;
                    String tFilePath = cs.getString(cs.getColumnIndex(FileColumns.DATA));
                    String tFolderPath = tFilePath.substring(0, tFilePath.lastIndexOf(File.separator));
                    String tFolderName = tFolderPath.substring(tFolderPath.lastIndexOf(File.separator) + 1);
                    int tFolderId = cs.getInt(cs.getColumnIndex(FileColumns.PARENT));
                    if (0 == tFolderId) {
                        tFolderName = "Root Folder";
                    }
                    FolderItem item = new FolderItem(tItemIndex, tFolderParentId, tFolderName, tFolderId);
                    list.add(item);
                    ++startIndex;
                } while (cs.moveToNext());
            }
        } catch (Exception ex) {
            Log.e(TAG, "ex:" + ex);
        }
        finally {
            if (cs != null) {
                cs.close();
            }
        }
        return list;
    }

    public int getFolderItemCount(int playPart, int folderId) {
        Log.d(TAG, "getFolderItemCount playPart = " + playPart + "folderId = " + folderId);
        Cursor cs = null;
        int count = 0;
        try {
            cs = queryFolder(playPart);
            if (cs != null && cs.getCount() != 0) {
                count = cs.getCount();
            }
        } catch (Exception ex) {
            Log.e(TAG, "ex:" + ex);
        }
        finally {
            if (cs != null) {
                cs.close();
            }
        }
        return count;
    }

    public List<FileItem> getFileList(int playPart, int folderId, int startIndex, int count) {
        Log.d(TAG, "getFileList playPart = " + playPart + "folderId = " + folderId + "startIndex = "
                + startIndex + "count = " + count);
        List<FileItem> list = new ArrayList<>();
        Cursor cs = null;

        try {
            cs = queryFile(playPart, folderId);
            if (cs != null && cs.getCount() != 0) {
                if (!cs.moveToPosition(startIndex)) {
                    Log.e(TAG, "Cursor moveToPosition(" + startIndex + ") failed. getCount=" + cs.getCount());
                    return list;
                }
                do {
                    if (list.size() >= count) {
                        break;
                    }
                    int tFileId = cs.getInt(cs.getColumnIndex(FileColumns._ID));
                    int tItemIndex = startIndex;
                    int tFolderParentId = folderId;
                    String tFilePath = cs.getString(cs.getColumnIndex(FileColumns.DATA));
                    String tFilename = tFilePath.substring(tFilePath.lastIndexOf(File.separator) + 1);
                    FileItem item = new FileItem(tFileId, tItemIndex, tFolderParentId, tFilename);
                    list.add(item);
                    ++startIndex;
                } while (cs.moveToNext());
            }
        } catch (Exception ex) {
            Log.e(TAG, "ex:" + ex);
        }
        finally {
            if (cs != null) {
                cs.close();
            }
        }
        return list;
    }

    public int getFileItemCount(int playPart, int folderId) {
        Log.d(TAG, "getFileItemCount playPart = " + playPart + "folderId = " + folderId);
        Cursor cs = null;
        int count = 0;
        try {
            cs = queryFile(playPart, folderId);
            if (cs != null && cs.getCount() != 0) {
                count = cs.getCount();
            }
        } catch (Exception ex) {
            Log.e(TAG, "ex:" + ex);
        }
        finally {
            if (cs != null) {
                cs.close();
            }
        }
        return count;
    }

    public ArrayList<Integer> getFolderList(int playPart) {
        Log.d(TAG, "getFolderList playPart = " + playPart);
        ArrayList<Integer> list = new ArrayList<>();
        Cursor cs = null;

        try {
            cs = queryFolder(playPart);
            if (cs != null && cs.getCount() != 0) {
                if (!cs.moveToFirst()) {
                    Log.e(TAG, "Cursor moveToFirst failed. getCount=" + cs.getCount());
                    return list;
                }
                do {
                    int tFolderId = cs.getInt(cs.getColumnIndex(FileColumns.PARENT));
                    Integer Id = new Integer(tFolderId);
                    list.add(Id);
                } while (cs.moveToNext());
            }
        } catch (Exception ex) {
            Log.e(TAG, "ex:" + ex);
        }
        finally {
            if (cs != null) {
                cs.close();
            }
        }
        return list;
    };

    public ArrayList<Integer> getFileListByFolder(int playPart, int folderId) {
        Log.d(TAG, "getFileListByFolder playPart = " + playPart + "folderId = " + folderId);
        ArrayList<Integer> list = new ArrayList<>();
        Cursor cs = null;

        try {
            cs = queryFile(playPart, folderId);
            if (cs != null && cs.getCount() != 0) {
                if (!cs.moveToFirst()) {
                    Log.e(TAG, "Cursor moveToFirst failed. getCount=" + cs.getCount());
                    return list;
                }
                do {
                    int tFildId = cs.getInt(cs.getColumnIndex(FileColumns._ID));
                    Integer Id = new Integer(tFildId);
                    list.add(Id);
                } while (cs.moveToNext());
            }
        } catch (Exception ex) {
            Log.e(TAG, "ex:" + ex);
        }
        finally {
            if (cs != null) {
                cs.close();
            }
        }
        return list;
    };

    public ArrayList<Integer> getAllFileList(int playPart) {
        Log.d(TAG, "getAllFileList playPart = " + playPart);
        ArrayList<Integer> list = new ArrayList<>();
        StringBuilder mSelection = null;
        if (IUsbDef.USB_PLAY_PART_AUDIO == playPart) {
            mSelection = new StringBuilder(FileColumns.DATA + " like'%.mp3' or " + FileColumns.DATA
                    + " like'%.wma' or " + FileColumns.DATA + " like'%.flac' or " + FileColumns.DATA
                    + " like'%.m4a' or " + FileColumns.DATA + " like'%.ape'");
        }
        else if (IUsbDef.USB_PLAY_PART_VIDEO == playPart) {
            mSelection = new StringBuilder(FileColumns.DATA + " like'%.3gp' or " + FileColumns.DATA
                    + " like'%.mp4' or " + FileColumns.DATA + " like'%.m4v' or " + FileColumns.DATA
                    + " like'%.avi' or " + FileColumns.DATA + " like'%.mov' or " + FileColumns.DATA
                    + " like'%.flv'");
        }
        else {
            return list;
        }
        ContentResolver cr = mContext.getContentResolver();
        Uri uri = MediaStore.Files.getContentUri("external");
        String[] proj = new String[]{FileColumns.DATA, FileColumns._ID};
        Cursor cs = null;

        try {
            cs = cr.query(uri, proj, mSelection.toString(), null, null);
            if (cs != null && cs.getCount() != 0) {
                if (!cs.moveToFirst()) {
                    Log.e(TAG, "Cursor moveToFirst failed. getCount=" + cs.getCount());
                    return list;
                }
                do {
                    int tFildId = cs.getInt(cs.getColumnIndex(FileColumns._ID));
                    Integer Id = new Integer(tFildId);
                    list.add(Id);
                } while (cs.moveToNext());
            }
        } catch (Exception ex) {
            Log.e(TAG, "ex:" + ex);
        }
        finally {
            if (cs != null) {
                cs.close();
            }
        }
        return list;
    };

    public int getFolderIDByFileID(int playPart, int fileId) {
        Log.d(TAG, "getFolderIDByFileID playPart = " + playPart + "fileId = " + fileId);
        StringBuilder mSelection = new StringBuilder(FileColumns._ID + " = " + fileId);
        ContentResolver cr = mContext.getContentResolver();
        Uri uri = MediaStore.Files.getContentUri("external");
        String[] proj = new String[]{FileColumns._ID, FileColumns.PARENT};
        Cursor cs = null;
        int tFildId = IUsbDef.USB_STATUS_FAIL;

        try {
            cs = cr.query(uri, proj, mSelection.toString(), null, null);
            if (cs != null && cs.getCount() != 0) {
                if (!cs.moveToFirst()) {
                    Log.e(TAG, "Cursor moveToFirst failed. getCount=" + cs.getCount());
                    return tFildId;
                }
                tFildId = cs.getInt(cs.getColumnIndex(FileColumns.PARENT));
            }
        } catch (Exception ex) {
            Log.e(TAG, "ex:" + ex);
        }
        finally {
            if (cs != null) {
                cs.close();
            }
        }
        return tFildId;
    }

    private Cursor queryFolder(int playPart) {
        StringBuilder mSelection = null;
        if (IUsbDef.USB_PLAY_PART_AUDIO == playPart) {
            mSelection = new StringBuilder(FileColumns.DATA + " like'%.mp3' or " + FileColumns.DATA
                    + " like'%.wma' or " + FileColumns.DATA + " like'%.flac' or " + FileColumns.DATA
                    + " like'%.m4a' or " + FileColumns.DATA + " like'%.ape'");
            mSelection.append(" ) group by ( " + FileColumns.PARENT);
        }
        else if (IUsbDef.USB_PLAY_PART_VIDEO == playPart) {
            mSelection = new StringBuilder(FileColumns.DATA + " like'%.3gp' or " + FileColumns.DATA
                    + " like'%.mp4' or " + FileColumns.DATA + " like'%.m4v' or " + FileColumns.DATA
                    + " like'%.avi' or " + FileColumns.DATA + " like'%.mov' or " + FileColumns.DATA
                    + " like'%.flv'");
            mSelection.append(" ) group by ( " + FileColumns.PARENT);
        }
        else {
            return null;
        }
        ContentResolver cr = mContext.getContentResolver();
        Uri uri = MediaStore.Files.getContentUri("external");
        String[] proj = new String[]{FileColumns.DATA, FileColumns.PARENT};
        Cursor cs = cr.query(uri, proj, mSelection.toString(), null, null);
        return cs;
    }

    private Cursor queryFile(int playPart, int folderId) {
        StringBuilder mSelection = null;
        if (IUsbDef.USB_PLAY_PART_AUDIO == playPart) {
            mSelection = new StringBuilder("(" + FileColumns.DATA + " like'%.mp3' or " + FileColumns.DATA
                    + " like'%.wma' or " + FileColumns.DATA + " like'%.flac' or " + FileColumns.DATA
                    + " like'%.m4a' or " + FileColumns.DATA + " like'%.ape') and " + FileColumns.PARENT
                    + " = " + folderId);
        }
        else if (IUsbDef.USB_PLAY_PART_VIDEO == playPart) {
            mSelection = new StringBuilder("(" + FileColumns.DATA + " like'%.3gp' or " + FileColumns.DATA
                    + " like'%.mp4' or " + FileColumns.DATA + " like'%.m4v' or " + FileColumns.DATA
                    + " like'%.avi' or " + FileColumns.DATA + " like'%.mov' or " + FileColumns.DATA
                    + " like'%.flv') and " + FileColumns.PARENT + " = " + folderId);
        }
        else {
            return null;
        }
        ContentResolver cr = mContext.getContentResolver();
        Uri uri = MediaStore.Files.getContentUri("external");
        String[] proj = new String[]{FileColumns._ID, FileColumns.DATA, FileColumns.PARENT};
        Cursor cs = cr.query(uri, proj, mSelection.toString(), null, null);
        return cs;
    }

} //UsbBrowserList