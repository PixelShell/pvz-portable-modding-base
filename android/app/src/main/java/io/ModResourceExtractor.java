package io.github.wszqkzqk.pvzportable;

import android.content.Context;
import java.io.*;
import java.nio.charset.StandardCharsets;

final class ModResourceExtractor {
    private static final String TAG = "PvZPortable";
    private static final int BUFFER_SIZE = 8192;

    static void extract(Context ctx, File gameDir) {
        File destDir = new File(gameDir, "resources");
        File versionMarker = new File(destDir, ".bundled_version");
        String currentVersion = String.valueOf(BuildConfig.VERSION_CODE);

        try {
            if (versionMarker.exists() && currentVersion.equals(readFirstLine(versionMarker))) {
                return;
            }
            String[] top = ctx.getAssets().list("resources");
            if (top == null || top.length == 0) return;
            copyAssetTree(ctx, "resources", destDir);
            writeFile(versionMarker, currentVersion);
        } catch (IOException e) {
            android.util.Log.e(TAG, "Failed to extract bundled mod resources", e);
        }
    }

    private static void copyAssetTree(Context ctx, String assetPath, File destDir) throws IOException {
        String[] children = ctx.getAssets().list(assetPath);
        if (children == null || children.length == 0) {
            destDir.getParentFile().mkdirs();
            try (InputStream is = ctx.getAssets().open(assetPath);
                 OutputStream os = new BufferedOutputStream(new FileOutputStream(destDir), BUFFER_SIZE)) {
                byte[] buf = new byte[BUFFER_SIZE];
                int len;
                while ((len = is.read(buf)) > 0) os.write(buf, 0, len);
            }
            return;
        }
        destDir.mkdirs();
        for (String child : children) copyAssetTree(ctx, assetPath + "/" + child, new File(destDir, child));
    }

    private static String readFirstLine(File file) throws IOException {
        try (BufferedReader reader = new BufferedReader(new FileReader(file))) {
            String line = reader.readLine();
            return line != null ? line.trim() : "";
        }
    }

    private static void writeFile(File file, String content) throws IOException {
        File parent = file.getParentFile();
        if (parent != null && !parent.exists()) parent.mkdirs();
        try (FileOutputStream fos = new FileOutputStream(file)) {
            fos.write(content.getBytes(StandardCharsets.UTF_8));
        }
    }
}