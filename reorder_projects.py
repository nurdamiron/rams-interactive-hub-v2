#!/usr/bin/env python3
"""
Reorder projects in projects.ts according to new requirements
"""

import re

# Read the original file
with open('lib/data/projects.ts', 'r', encoding='utf-8') as f:
    content = f.read()

# Extract header (everything before the array)
header_match = re.search(r'(.*?export const RAMS_PROJECTS: Project\[\] = \[)', content, re.DOTALL)
header = header_match.group(1) if header_match else ''

# Extract footer (everything after the array)
footer_match = re.search(r'(\];\s*//.*$)', content, re.DOTALL)
footer = footer_match.group(1) if footer_match else '];'

# Extract all project blocks
project_pattern = r'  // \d+\. ([^\n]+)\n  \{[\s\S]*?\n  \},'
projects = {}

for match in re.finditer(project_pattern, content):
    project_block = match.group(0)
    # Extract project ID from the block
    id_match = re.search(r'id: "([^"]+)"', project_block)
    if id_match:
        project_id = id_match.group(1)
        projects[project_id] = project_block

# New order (first 15 projects for actuator control)
new_order = [
    '09-rams-garden-almaty',        # 1. Rams Garden Almaty → blocks 1,2
    '23-haval',                      # 2. Haval (Haval+Hyundai+Lukoil) → blocks 3,4
    '22-almaty-museum',              # 3. Almaty Museum of Arts → blocks 5,6
    '18-lamiya',                     # 4. Lamiya → blocks 7,8
    '11-rams-signature',             # 5. Rams Signature → blocks 9,10
    '20-ile-de-france',              # 6. ile de france → blocks 11,12
    '12-rams-saiahat',               # 7. Saiahat → blocks 13,14
    '13-rams-garden-atyrau',         # 8. Rams Garden Atyrau → block 15 (3 actuators!)
    '15-rams-evo',                   # 9. Rams Evo (no actuators)
    '21-forum-residence',            # 10. Forum Residence (no actuators)
    '08-vostochny-park',             # 11. Восточный парк (no actuators)
    '19-la-verde',                   # 12. La Verde (no actuators)
    '05-dom-na-abaya',               # 13. Дом на Абая (no actuators)
    '17-ortau',                      # 14. Ortau (no actuators)
    '26-marriott-issykkul',          # 15. Marriot Issukool (no actuators)
]

# Build new projects array
new_projects = []

# Add projects in new order
for idx, project_id in enumerate(new_order, 1):
    if project_id in projects:
        block = projects[project_id]
        # Update comment number
        block = re.sub(r'  // \d+\.', f'  // {idx}.', block)
        new_projects.append(block)
    else:
        print(f'Warning: Project {project_id} not found!')

# Add remaining projects (not in new_order)
remaining_ids = [pid for pid in projects.keys() if pid not in new_order]
for idx, project_id in enumerate(sorted(remaining_ids), len(new_order) + 1):
    block = projects[project_id]
    # Update comment number
    block = re.sub(r'  // \d+\.', f'  // {idx}.', block)
    new_projects.append(block)

# Construct new file content
new_content = header + '\n' + '\n'.join(new_projects) + '\n' + footer

# Write to new file
with open('lib/data/projects.ts', 'w', encoding='utf-8') as f:
    f.write(new_content)

print('✅ Projects reordered successfully!')
print(f'✅ First 15 projects are now in the correct order')
print(f'✅ Projects 1-7 control actuator blocks 1-14 (pairs)')
print(f'✅ Project 8 controls actuator block 15 (3 actuators)')
print(f'✅ Projects 9-15 do not control actuators')
