import android.os.Parcel;
import android.os.Parcelable;

public final class FolderItem implements Parcelable {
    private int mFolderId;
    private int mItemIndex;
    private int mFolderParentId;
    private String mFolderName;

    public static final Parcelable.Creator<FolderItem> CREATOR = new Parcelable.Creator<FolderItem>() {
        public FolderItem createFromParcel(Parcel in) {
            return new FolderItem(in);
        }

        public FolderItem[] newArray(int size) {
            return new FolderItem[size];
        }
    };

    public FolderItem(int itemIndex,
                      int folderParentId,
                      String folderName,
                      int folderId) {
        mItemIndex = itemIndex;
        mFolderParentId = folderParentId;
        mFolderName = folderName;
        mFolderId = folderId;
    }

    private FolderItem(Parcel in) {
        readFromParcel(in);
    }

    @Override
    public void writeToParcel(Parcel out, int flags) {
        out.writeInt(mItemIndex);
        out.writeInt(mFolderParentId);
        out.writeString(mFolderName);
        out.writeInt(mFolderId);
    }

    public void readFromParcel(Parcel in) {
        mItemIndex = in.readInt();
        mFolderParentId = in.readInt();
        mFolderName = in.readString();
        mFolderId = in.readInt();
    }

    @Override
    public int describeContents() {
        return 0;
    }

    public int getItemIndex() {
        return mItemIndex;
    }

    public int getFolderParentId() {
        return mFolderParentId;
    }

    public String getFolderName() {
        return mFolderName;
    }

    public int getFolderId() {
        return mFolderId;
    }

    public void setItemIndex(int itemIndex) {
        mItemIndex = itemIndex;
    }

    public void setFolderParentId(int folderParentId) {
        mFolderParentId = folderParentId;
    }

    public void setFolderName(String folderName) {
        mFolderName = folderName;
    }

    public void setFolderId(int folderId) {
        mFolderId = folderId;
    }
}