import os
import shutil
import subprocess
import re

SOURCE_DIR = "/Users/nurdauletakhmatov/Documents/logo render krug2"
TARGET_BASE_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "public", "projects"))

# Mapping of source folders to target project IDs
MAPPING = {
    "Nomad,": "01-nomad",
    "Grande Vie,": "02-grande-vie",
    "Keruen City,": "03-keruen-city",
    "Rams Garden Bahcelievler,": "04-rams-garden-bahcelievler",
    "Rams Resort Bodrum": "05-rams-resort-bodrum",
    "Rams City Halic 2": "06-rams-city-halic-2",
    "Park House Maslak,": "07-park-house-maslak",
    "Sakura,": "08-sakura",
    "Rams City Halic 1,": "09-rams-city-halic-1",
    "Rams City Gazientep,": "10-rams-city-gaziantep",
    "Baiterek School,": "11-baiterek-school",
    "Hyatt Regency,": "12-hyatt-regency",
    "Rams City Almaty,": "13-rams-city-almaty"
}

def clean_and_create_dir(dir_path):
    if os.path.exists(dir_path):
        # Remove everything except _README.txt
        for item in os.listdir(dir_path):
            item_path = os.path.join(dir_path, item)
            if item == "_README.txt":
                continue
            if os.path.isdir(item_path):
                shutil.rmtree(item_path)
            else:
                os.remove(item_path)
    else:
        os.makedirs(dir_path, exist_ok=True)

def convert_avif_to_jpg(src, dst):
    print(f"  Converting AVIF using ffmpeg: {os.path.basename(src)} -> {os.path.basename(dst)}")
    cmd = ["/opt/homebrew/bin/ffmpeg", "-i", src, "-update", "1", "-frames:v", "1", dst, "-y"]
    subprocess.run(cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, check=True)

def convert_tif_to_jpg(src, dst):
    print(f"  Converting TIF using sips: {os.path.basename(src)} -> {os.path.basename(dst)}")
    cmd = ["/usr/bin/sips", "-s", "format", "jpeg", src, "--out", dst]
    subprocess.run(cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, check=True)

def main():
    print("Starting media copy and conversion script...")
    
    if not os.path.exists(SOURCE_DIR):
        print(f"Error: Source directory {SOURCE_DIR} does not exist!")
        return
        
    for src_folder, target_id in MAPPING.items():
        src_path = os.path.join(SOURCE_DIR, src_folder)
        if not os.path.exists(src_path):
            # Try to match without trailing comma
            src_path_no_comma = os.path.join(SOURCE_DIR, src_folder.rstrip(','))
            if os.path.exists(src_path_no_comma):
                src_path = src_path_no_comma
            else:
                print(f"Warning: Source folder '{src_folder}' not found. Skipping...")
                continue
                
        print(f"\nProcessing project: {target_id} (from {os.path.basename(src_path)})")
        
        target_project_dir = os.path.join(TARGET_BASE_DIR, target_id)
        clean_and_create_dir(target_project_dir)
        
        # Subdirectories
        images_dir = os.path.join(target_project_dir, "images")
        logo_dir = os.path.join(images_dir, "logo")
        scenes_dir = os.path.join(images_dir, "scenes")
        videos_dir = os.path.join(target_project_dir, "videos")
        
        os.makedirs(images_dir, exist_ok=True)
        os.makedirs(logo_dir, exist_ok=True)
        os.makedirs(scenes_dir, exist_ok=True)
        os.makedirs(videos_dir, exist_ok=True)
        
        # Scan source files
        files = [f for f in os.listdir(src_path) if f != ".DS_Store" and os.path.isfile(os.path.join(src_path, f))]
        
        # Identify logo
        logo_file = None
        # Rule 1: contains "logo"
        for f in files:
            if "logo" in f.lower():
                logo_file = f
                break
        
        # Rule 2: if no logo contains "logo", but there is a .svg file (e.g. sakura.svg)
        if not logo_file:
            for f in files:
                if f.lower().endswith(".svg"):
                    logo_file = f
                    break
                    
        # Rule 3: if still no logo, but there's a WhatsApp image or a very small file
        if not logo_file:
            for f in files:
                if "whatsapp" in f.lower() or os.path.getsize(os.path.join(src_path, f)) < 150000:
                    # check if it's an image
                    if f.lower().endswith((".jpg", ".jpeg", ".png", ".webp")):
                        logo_file = f
                        break
                        
        # Copy/convert logo
        if logo_file:
            src_logo_path = os.path.join(src_path, logo_file)
            ext = os.path.splitext(logo_file)[1].lower()
            if ext == ".svg":
                dest_logo_name = "logo.svg"
                shutil.copy2(src_logo_path, os.path.join(logo_dir, dest_logo_name))
                print(f"  Copied SVG logo: {logo_file} -> logo.svg")
            elif ext in [".jpg", ".jpeg"]:
                dest_logo_name = "logo.jpg"
                shutil.copy2(src_logo_path, os.path.join(logo_dir, dest_logo_name))
                print(f"  Copied JPG logo: {logo_file} -> logo.jpg")
            elif ext == ".png":
                dest_logo_name = "logo.png"
                shutil.copy2(src_logo_path, os.path.join(logo_dir, dest_logo_name))
                print(f"  Copied PNG logo: {logo_file} -> logo.png")
            else:
                dest_logo_name = "logo.jpg"
                convert_tif_to_jpg(src_logo_path, os.path.join(logo_dir, dest_logo_name))
                print(f"  Converted & Copied logo: {logo_file} -> logo.jpg")
        else:
            # Fallback logo
            placeholder_src = os.path.abspath(os.path.join(TARGET_BASE_DIR, "..", "images", "logo-placeholder.svg"))
            if os.path.exists(placeholder_src):
                shutil.copy2(placeholder_src, os.path.join(logo_dir, "logo.svg"))
                print("  No logo found, copied logo-placeholder.svg")
            else:
                print("  Warning: No logo or fallback found!")
                
        # Identify scenes (all files except logo_file)
        scene_files = [f for f in files if f != logo_file]
        # Filter only image files
        scene_files = [f for f in scene_files if f.lower().endswith((".jpg", ".jpeg", ".png", ".webp", ".avif", ".tif", ".tiff"))]
        scene_files.sort()
        
        # Process scene images
        for idx, f in enumerate(scene_files):
            src_file_path = os.path.join(src_path, f)
            ext = os.path.splitext(f)[1].lower()
            
            # First scene is main.jpg
            if idx == 0:
                dest_path = os.path.join(images_dir, "main.jpg")
                if ext == ".avif":
                    convert_avif_to_jpg(src_file_path, dest_path)
                elif ext in [".tif", ".tiff"]:
                    convert_tif_to_jpg(src_file_path, dest_path)
                else:
                    shutil.copy2(src_file_path, dest_path)
                    print(f"  Copied main image: {f} -> main.jpg")
            else:
                # Other scenes go to images/scenes/
                scene_num = f"{idx:02d}"  # 01, 02, etc.
                dest_ext = ".jpg" if ext in [".tif", ".tiff", ".avif"] else ext
                dest_name = f"{scene_num}{dest_ext}"
                dest_path = os.path.join(scenes_dir, dest_name)
                
                if ext == ".avif":
                    convert_avif_to_jpg(src_file_path, dest_path)
                elif ext in [".tif", ".tiff"]:
                    convert_tif_to_jpg(src_file_path, dest_path)
                else:
                    shutil.copy2(src_file_path, dest_path)
                    print(f"  Copied scene: {f} -> {dest_name}")
                    
    print("\nMedia copying and conversion finished successfully!")

if __name__ == "__main__":
    main()
