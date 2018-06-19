
import android.os.Parcel;
import android.os.Parcelable;

public final class FileItem implements Parcelable {
    private int mFileId;
    private int mItemIndex;
    private int mFolderParentId;
    private String mFilename;

    public static final Parcelable.Creator<FileItem> CREATOR = new Parcelable.Creator<FileItem>() {
        public FileItem createFromParcel(Parcel in) {
            return new FileItem(in);
        }

        public FileItem[] newArray(int size) {
            return new FileItem[size];
        }
    };

    public FileItem(int fileId,
                    int itemIndex,
                    int folderParentId,
                    String filename) {
        mFileId = fileId;
        mItemIndex = itemIndex;
        mFolderParentId = folderParentId;
        mFilename = filename;
    }

    private FileItem(Parcel in) {
        readFromParcel(in);
    }

    @Override
    public void writeToParcel(Parcel out, int flags) {
        out.writeInt(mFileId);
        out.writeInt(mItemIndex);
        out.writeInt(mFolderParentId);
        out.writeString(mFilename);
    }

    public void readFromParcel(Parcel in) {
        mFileId = in.readInt();
        mItemIndex = in.readInt();
        mFolderParentId = in.readInt();
        mFilename = in.readString();
    }

    @Override
    public int describeContents() {
        return 0;
    }

    public int getFileID() {
        return mFileId;
    }

    public int getItemIndex() {
        return mItemIndex;
    }

    public int getFolderParentId() {
        return mFolderParentId;
    }

    public String getFilename() {
        return mFilename;
    }

    public void setFileID(int fileId) {
        mFileId = fileId;
    }

    public void setItemIndex(int itemIndex) {
        mItemIndex = itemIndex;
    }

    public void setFolderParentId(int folderParentId) {
        mFolderParentId = folderParentId;
    }

    public void setFilename(String filename) {
        mFilename = filename;
    }
}