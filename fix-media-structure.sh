#!/bin/bash
# Script to adapt old media structure to new project structure

PROJECT_DIR="/Users/nurdauletakhmatov/Desktop/rams-screen/rams-interactive-hub/rams-interactive-hub/public/projects"

# Function to fix project structure
fix_project() {
  local old_name=$1
  local new_name=$2

  echo "Processing: $new_name (from $old_name)"

  cd "$PROJECT_DIR/$new_name" 2>/dev/null || return

  # Create main.jpg symlink from hero/ or scenes/ГЛАВНЫЙ ФОТО.jpg
  if [ -f "images/hero/main.jpg" ]; then
    ln -sf hero/main.jpg images/main.jpg 2>/dev/null
  elif [ -f "images/scenes/ГЛАВНЫЙ ФОТО.jpg" ]; then
    ln -sf "scenes/ГЛАВНЫЙ ФОТО.jpg" images/main.jpg 2>/dev/null
  fi

  # Fix logo symlinks
  if [ -d "images/logo" ]; then
    cd images/logo
    # Find any .svg or .png file and create logo symlink
    for file in *.svg *.png; do
      if [ -f "$file" ] && [ "$file" != "logo.svg" ] && [ "$file" != "logo.png" ]; then
        ext="${file##*.}"
        ln -sf "$file" "logo.$ext" 2>/dev/null
        break
      fi
    done
    cd ../..
  fi

  # Number scenes if they exist
  if [ -d "images/scenes" ]; then
    cd images/scenes
    count=1
    for file in *.jpg *.jpeg *.png; do
      if [ -f "$file" ] && [ "$file" != "ГЛАВНЫЙ ФОТО.jpg" ]; then
        num=$(printf "%02d" $count)
        if [ ! -f "$num.jpg" ]; then
          ln -sf "$file" "$num.jpg" 2>/dev/null
        fi
        ((count++))
      fi
    done
    cd ../..
  fi

  echo "  ✓ Fixed $new_name"
}

# Fix all projects
fix_project "dom-na-abaya" "05-dom-na-abaya"
fix_project "vostochny-park" "08-vostochny-park"
fix_project "rams-garden" "09-rams-garden-almaty"
fix_project "rams-signature" "11-rams-signature"
fix_project "rams-saiahat" "12-rams-saiahat"
fix_project "rams-garden-atyrau" "13-rams-garden-atyrau"
fix_project "bc-ortau-marriott" "14-ortau-marriott-bc"
fix_project "rams-evo" "15-rams-evo"
fix_project "ortau-jk" "17-ortau"
fix_project "lamiya" "18-lamiya"
fix_project "la-verde" "19-la-verde"
fix_project "ile-de-france" "20-ile-de-france"
fix_project "forum-residence" "21-forum-residence"
fix_project "almaty-museum" "22-almaty-museum"
fix_project "lukoil" "24-lukoil"
fix_project "marriott-issykkul" "26-marriott-issykkul"
fix_project "hyundai" "27-hyundai"

echo ""
echo "✅ All projects fixed!"
echo "Now refresh your browser: http://localhost:3000"
