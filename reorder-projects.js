// Script to reorder projects according to new requirements
const fs = require('fs');
const path = require('path');

// Read the current projects file
const projectsFilePath = path.join(__dirname, 'lib/data/projects.ts');
const content = fs.readFileSync(projectsFilePath, 'utf-8');

// New order of project IDs (according to user's list)
const newOrder = [
  '09-rams-garden-almaty',        // 1. Rams Garden Almaty
  '23-haval',                      // 2. Haval Hyundai Lukoil (using Haval as representative)
  '22-almaty-museum',              // 3. Almaty Museum of Arts
  '18-lamiya',                     // 4. Lamiya
  '11-rams-signature',             // 5. Rams Signature
  '20-ile-de-france',              // 6. ile de france
  '12-rams-saiahat',               // 7. Saiahat
  '13-rams-garden-atyrau',         // 8. Rams Garden Atyrau (BLOCK 15 - 3 actuators)
  '15-rams-evo',                   // 9. Rams Evo
  '21-forum-residence',            // 10. Forum Residence
  '08-vostochny-park',             // 11. Восточный парк
  '19-la-verde',                   // 12. La Verde
  '05-dom-na-abaya',               // 13. Дом на Абая
  '17-ortau',                      // 14. Ortau
  '26-marriott-issykkul',          // 15. Marriot Issukool
];

console.log('New project order:');
newOrder.forEach((id, index) => {
  console.log(`${index + 1}. ${id}`);
});

console.log('\nNote: For "Haval Hyundai Lukoil", using Haval (id: 23-haval) as representative.');
console.log('Projects 9-15 will not control actuators (only first 8 projects have actuator mapping).');
console.log('\nActuator mapping:');
console.log('  Project 1 (Rams Garden Almaty) → blocks 1,2');
console.log('  Project 2 (Haval) → blocks 3,4');
console.log('  Project 3 (Almaty Museum) → blocks 5,6');
console.log('  Project 4 (Lamiya) → blocks 7,8');
console.log('  Project 5 (Rams Signature) → blocks 9,10');
console.log('  Project 6 (ile de france) → blocks 11,12');
console.log('  Project 7 (Saiahat) → blocks 13,14');
console.log('  Project 8 (Rams Garden Atyrau) → block 15 (3 actuators)');
console.log('  Projects 9-15 → no actuators');
