/* omega, (C) 1987,1988,1989 by Laurence Raphael Brothers */
/* iinit.h */
/* This file defines and initializes the Objects Array */

struct object Objects[TOTALITEMS] = {
{ 0,10,0,0,0,0,0,1,10,50,0,0,0,0,COMMON,I_RAISE_PORTCULLIS,2,THING,"small grey box with red button","disposeable garage door opener","disposeable garage door opener" },

{ 1,20,0,0,0,0,0,1,20,2000,0,0,0,0,COMMON,I_NO_OP,4,THING,"some rocks","uncut diamonds","uncut diamonds" },

{ 2,15,0,0,0,0,0,1,10,50,0,0,0,0,COMMON,I_PICK,2,THING,"twisted piece of metal","thieve's pick","thieve's pick" },

{ 3,10,0,0,0,0,0,1,15,1500,0,0,0,0,COMMON,I_NO_OP,1,THING,"large green gem","humongous emerald","humongous emerald" },

{ 4,200,0,0,0,0,0,1,18,1750,0,0,0,0,COMMON,I_NO_OP,4,THING,"gold ingot","gold ingot","gold ingot" },

{ 5,5,0,0,0,0,0,1,1,3000,0,0,0,0,COMMON,I_NO_OP,5,THING,"old illegible documents","bearer bonds","bearer bonds" },

{ 6,100,0,0,0,0,0,1,1,1,0,0,0,0,COMMON,I_NO_OP,0,THING,"bucket of salt water","bucket of salt water","bucket of salt water" },

{ 7,3,0,0,0,0,0,1,10,20,0,0,0,0,COMMON,I_KEY,2,THING,"key","magic key","magic key" },

{ 8,30,0,0,0,0,6,1,8,10,0,0,0,0,COMMON,I_PERM_ILLUMINATE,0,THING,"torch","torch","torch" },

{ 9,10,0,0,0,0,0,1,12,0,0,0,0,0,COMMON,I_NO_OP,0,THING,"grot","grot","grot" },

{ 10,10,0,0,0,0,0,1,12,0,0,0,0,0,COMMON,I_NO_OP,0,THING,"grot","grot","grot" },

{ 11,10,0,0,0,0,0,1,12,0,0,0,0,0,COMMON,I_NO_OP,0,THING,"grot","grot","grot" },

{ 12,10,0,0,0,0,0,1,12,0,0,0,0,0,COMMON,I_NO_OP,0,THING,"grot","grot","grot" },

{ 13,10,0,0,0,0,0,1,12,0,0,0,0,0,COMMON,I_NO_OP,0,THING,"grot","grot","grot" },

{ 14,10,0,0,0,0,0,1,12,0,0,0,0,0,COMMON,I_NO_OP,0,THING,"grot","grot","grot" },

{ 15,10,0,0,0,0,0,1,12,0,0,0,0,0,COMMON,I_NO_OP,0,THING,"grot","grot","grot" },

{ 16,10,0,0,0,0,0,1,12,0,0,0,0,0,UNIQUE_MADE,I_NO_OP,0,THING,"Justiciar's Badge","Justiciar's Badge","Justiciar's Badge" },

{ 17,10,0,0,0,0,89,1,3,30,0,0,0,0,COMMON,I_TRAP,0,THING,"spring-loaded tube","dart-trap component","dart-trap component" },

{ 18,10,0,0,0,0,98,1,3,30,0,0,0,0,COMMON,I_TRAP,0,THING,"pressurized cannister","acid-trap component","acid-trap component" },

{ 19,10,0,0,0,0,92,1,3,30,0,0,0,0,COMMON,I_TRAP,0,THING,"coil of fine wire","snare component","snare component" },

{ 20,10,0,0,0,0,94,1,3,30,0,0,0,0,COMMON,I_TRAP,0,THING,"mound of powdered aluminum","fire-trap component","fire-trap component" },

{ 21,10,0,0,0,0,95,1,3,30,0,0,0,0,COMMON,I_TRAP,0,THING,"powerful rune","teleport-trap component","teleport-trap component" },

{ 22,10,0,0,0,0,97,1,3,30,0,0,0,0,COMMON,I_TRAP,0,THING,"pressurized cannister","sleepgas-trap component","sleepgas-trap component" },

{ 23,10,0,0,0,0,96,1,3,30,0,0,0,0,COMMON,I_TRAP,0,THING,"powerful rune","disintegration-trap component","disintegration-trap component" },

{ 24,10,0,0,0,0,100,1,3,30,0,0,0,0,COMMON,I_TRAP,0,THING,"dark fabric patch","abyss-trap component","abyss-trap component" },

{ 25,10,0,0,0,0,99,1,3,30,0,0,0,0,COMMON,I_TRAP,0,THING,"powerful rune","manadrain-trap component","manadrain-trap component" },

{ 26,20,0,0,0,0,8,1,0,2,0,0,0,0,COMMON,I_FOOD,0,FOOD,"red and white striped bucket","food ration","food ration" },

{ 27,10,0,0,0,0,48,1,0,200,0,0,0,0,COMMON,I_LEMBAS,2,FOOD,"wafer of lembas","wafer of lembas","wafer of lembas" },

{ 28,1,0,0,0,0,1,1,0,50,0,0,0,0,COMMON,I_STIM,3,FOOD,"small gray tablet","stimtab","stimtab" },

{ 29,1,0,0,0,0,1,1,0,100,0,0,0,0,COMMON,I_POW,3,FOOD,"small gray tablet","powtab","powtab" },

{ 30,1,0,0,0,0,36,1,0,20,0,0,0,0,COMMON,I_FOOD,3,FOOD,"small gray tablet","nutritab","nutritab" },

{ 31,1,0,0,0,0,1,1,0,500,0,0,0,0,COMMON,I_IMMUNE,3,FOOD,"small gray tablet","immunotab","immunotab" },

{ 32,1,0,0,0,0,2,1,0,1,0,0,0,0,COMMON,I_FOOD,0,FOOD,"jelly baby","jelly baby","jelly baby" },

{ 33,20,0,0,0,0,-1,1,0,25,0,0,0,0,COMMON,I_POISON_FOOD,1,FOOD,"food ration","poisoned ration","poisoned ration" },

{ 34,2,0,0,0,0,1,1,0,100,0,0,0,0,COMMON,I_PEPPER_FOOD,2,FOOD,"withered reddish stringy vegetable","szechuan pepper","szechuan pepper" },

{ 35,2,0,0,0,0,8,1,0,5,0,0,0,0,COMMON,I_FOOD,2,FOOD,"messy red and white disk","pepperoni pizza pie","pepperoni pizza pie" },

{ 36,2,0,0,0,0,6,1,0,7,0,0,0,0,COMMON,I_FOOD,2,FOOD,"brown bag with strange ideograms","hunan take-out","hunan take-out" },

{ 37,2,0,0,0,0,2,1,0,5,0,0,0,0,COMMON,I_FOOD,2,FOOD,"strange foamy cylinders","twinkies","twinkies" },

{ 38,2,0,0,0,0,3,1,0,3,0,0,0,0,COMMON,I_FOOD,2,FOOD,"squiggly orange spirals","cheese-doodles","cheese-doodles" },

{ 39,2,0,0,0,0,4,1,0,3,0,0,0,0,COMMON,I_FOOD,2,FOOD,"sweet nutty-smelling disks","pecan twirls","pecan_twirls" },

{ 40,2,0,0,0,0,10,1,0,75,0,0,0,0,COMMON,I_FOOD,2,FOOD,"pale white strips with green dots","veal piccata with capers","veal piccata with capers" },

{ 41,2,0,0,0,0,3,1,0,5,0,0,0,0,COMMON,I_FOOD,2,FOOD,"a bag of hard brown kernels","a bag of unmilled grain","a bag of unmilled grain" },

{ 42,5,0,0,0,0,0,1,1,5,0,0,0,0,COMMON,I_NOTHING,0,SCROLL,"?","blank scroll","Antinomy!" },

{ 43,5,0,0,0,0,0,1,1,250,0,0,0,0,COMMON,I_SPELLS,1,SCROLL,"?","scroll of spells","scroll of spell loss" },

{ 44,5,0,0,0,0,0,1,1,400,0,0,0,0,COMMON,I_BLESS,2,SCROLL,"?","scroll of blessing","scroll of cursing" },

{ 45,5,0,0,0,0,0,1,1,5000,0,0,0,0,COMMON,I_ACQUIRE,7,SCROLL,"?","scroll of acquirement","scroll of destruction" },

{ 46,5,0,0,0,0,0,1,1,200,0,0,0,0,COMMON,I_ENCHANT,2,SCROLL,"?","scroll of enchantment","scroll of disenchantment" },

{ 47,5,0,0,0,0,0,1,1,75,0,0,0,0,COMMON,I_TELEPORT,1,SCROLL,"?","scroll of teleportation","scroll of teleportation" },

{ 48,5,0,0,0,0,0,1,1,10000,0,0,0,0,COMMON,I_WISH,10,SCROLL,"?","scroll of wishing","scroll of wishing" },

{ 49,5,0,0,0,0,0,1,1,25,0,0,0,0,COMMON,I_CLAIRVOYANCE,1,SCROLL,"?","scroll of clairvoyance","scroll of amnesia" },

{ 50,5,0,0,0,0,0,1,1,50,0,0,0,0,COMMON,I_DISPLACE,2,SCROLL,"?","scroll of displacement","scroll of vulnerability" },

{ 51,5,0,0,0,0,0,1,1,20,0,0,0,0,COMMON,I_ID,0,SCROLL,"?","scroll of identification","scroll of forgetfulness" },

{ 52,5,0,0,0,0,0,1,1,10,0,0,0,0,COMMON,I_HINT,0,SCROLL,"?","hint sheet","hint sheet" },

{ 53,5,0,0,0,0,0,1,1,50,0,0,0,0,COMMON,I_JANE_T,2,SCROLL,"?","Jane's guide to treasure","Jane's guide to treasure" },

{ 54,5,0,0,0,0,0,1,1,50,0,0,0,0,COMMON,I_ALERT,1,SCROLL,"?","scroll of alertness","scroll of drowsiness" },

{ 55,5,0,0,0,0,0,1,1,300,0,0,0,0,COMMON,I_FLUX,5,SCROLL,"?","scroll of cosmic flux","scroll of cosmic flux" },

{ 56,5,0,0,0,0,0,1,1,100,0,0,0,0,COMMON,I_CHARGE,2,SCROLL,"?","scroll of charging","scroll of draining" },

{ 57,5,0,0,0,0,0,1,1,100,0,0,0,0,COMMON,I_WARP,3,SCROLL,"?","scroll of the warp","scroll of the warp" },

{ 58,5,0,0,0,0,0,1,1,10,0,0,0,0,COMMON,I_KNOWLEDGE,1,SCROLL,"?","scroll of self-knowledge","scroll of delusion" },

{ 59,5,0,0,0,0,0,1,1,100,0,0,0,0,COMMON,I_LAW,2,SCROLL,"?","scroll of law","scroll of law" },

{ 60,5,0,0,0,0,0,1,1,200,0,0,0,0,COMMON,I_HERO,5,SCROLL,"?","scroll of heroism","scroll of cowardliness" },

{ 61,5,0,0,0,0,0,1,1,50,0,0,0,0,COMMON,I_TRUESIGHT,2,SCROLL,"?","scroll of true sight","scroll of blindness" },

{ 62,5,0,0,0,0,0,1,1,250,0,0,0,0,COMMON,I_DISPEL,3,SCROLL,"?","scroll of dispelling","scroll of self-dispelling" },

{ 63,5,0,0,0,0,0,1,1,300,0,0,0,0,COMMON,I_SUMMON,2,SCROLL,"?","scroll of summoning","scroll of summoning" },

{ 64,5,0,0,0,0,0,1,1,30,0,0,0,0,COMMON,I_ILLUMINATE,0,SCROLL,"?","scroll of light","scroll of darkness" },

{ 65,5,0,0,0,0,0,1,1,250,0,0,0,0,COMMON,I_DEFLECT,3,SCROLL,"?","scroll of deflection","scroll of vulnerability" },

{ 66,20,0,0,0,0,0,1,5,0,0,0,0,0,COMMON,I_NO_OP,0,POTION,"?","potion of quenching thirst","Antinomy!" },

{ 67,20,0,0,0,0,0,1,5,40,0,0,0,0,COMMON,I_HEAL,2,POTION,"?","potion of healing","potion of disruption" },

{ 68,20,0,0,0,0,0,1,5,10,0,0,0,0,COMMON,I_OBJDET,1,POTION,"?","potion of object detection","potion of object detection" },

{ 69,20,0,0,0,0,0,1,5,10,0,0,0,0,COMMON,I_MONDET,1,POTION,"?","potion of monster detection","potion of monster detection" },

{ 70,20,0,0,0,0,0,1,5,20,0,0,0,0,COMMON,I_NEUTRALIZE_POISON,2,POTION,"?","potion of neutralize poison","potion of poison" },

{ 71,20,0,0,0,0,0,1,5,100,0,0,0,0,COMMON,I_ALERT,0,POTION,"?","potion of alertness","potion of sleep" },

{ 72,20,0,0,0,0,0,1,5,40,0,0,0,0,COMMON,I_SPEED,1,POTION,"?","potion of speed","potion of slowness" },

{ 73,20,0,0,0,0,0,1,5,50,0,0,0,0,COMMON,I_RESTORE,3,POTION,"?","potion of restoration","potion of debilitation" },

{ 74,20,-1,0,0,0,0,1,5,1500,0,0,0,0,COMMON,I_AZOTH,7,POTION,"mercury","Azoth","mercury" },

{ 75,20,0,0,0,0,0,1,5,100,0,0,0,0,COMMON,I_REGENERATE,4,POTION,"?","potion of regeneration","potion of great disruption" },

{ 76,20,0,0,0,0,0,1,5,100,0,0,0,0,COMMON,I_INVISIBLE,2,POTION,"?","potion of invisibility","potion of aggravation" },

{ 77,20,0,0,0,0,0,1,5,30,0,0,0,0,COMMON,I_BREATHING,2,POTION,"?","potion of breathing","potion of choking" },

{ 78,20,0,0,0,0,0,1,5,200,0,0,0,0,COMMON,I_AUGMENT,5,POTION,"?","potion of augmentation","potion of debilitation" },

{ 79,20,0,0,0,0,0,1,5,50,0,0,0,0,COMMON,I_FEAR_RESIST,2,POTION,"?","potion of banishing fear","potion of fear" },

{ 80,20,0,0,0,0,0,1,5,100,0,0,0,0,COMMON,I_CHAOS,3,POTION,"?","potion of chaos","Antinomy!" },

{ 81,20,0,0,0,0,0,1,5,200,0,0,0,0,COMMON,I_ACCURACY,3,POTION,"?","potion of accuracy","potion of fuzziness" },

{ 82,20,0,0,0,0,0,1,5,50,0,0,0,0,COMMON,I_LEVITATION,1,POTION,"?","potion of levitation","potion of levitation" },

{ 83,20,0,0,0,0,0,1,5,50,0,0,0,0,COMMON,I_CURE,1,POTION,"?","potion of curing","potion of disease" },

{ 84,10,0,0,6,12,0,1,15,5,0,0,0,THRUSTING,COMMON,I_NORMAL_WEAPON,0,WEAPON,"dagger","dagger","dagger" },

{ 85,25,0,0,8,11,0,1,15,40,0,0,0,CUTTING,COMMON,I_NORMAL_WEAPON,1,WEAPON,"short sword","short sword","short sword" },

{ 86,40,0,0,12,9,0,1,15,90,0,0,0,CUTTING,COMMON,I_NORMAL_WEAPON,2,WEAPON,"broad sword","broad sword","broad sword" },

{ 87,50,0,0,16,8,0,1,15,250,0,0,0,CUTTING,COMMON,I_NORMAL_WEAPON,3,WEAPON,"bastard sword","bastard sword","bastard sword" },

{ 88,50,0,0,16,10,0,1,15,500,0,0,0,CUTTING,COMMON,I_NORMAL_WEAPON,4,WEAPON,"katana","katana","katana" },

{ 89,75,0,0,20,7,0,1,15,400,0,0,0,CUTTING,COMMON,I_NORMAL_WEAPON,5,WEAPON,"great-sword","great-sword","great-sword" },

{ 90,15,0,0,6,15,0,1,15,50,0,0,0,THRUSTING,COMMON,I_NORMAL_WEAPON,2,WEAPON,"epee","epee","epee" },

{ 91,20,0,0,8,13,0,1,15,250,0,0,0,THRUSTING,COMMON,I_NORMAL_WEAPON,3,WEAPON,"rapier","rapier","rapier" },

{ 92,25,0,0,11,12,0,1,15,500,0,0,0,THRUSTING,COMMON,I_NORMAL_WEAPON,5,WEAPON,"estoc","estoc","estoc" },

{ 93,35,0,0,8,8,0,1,15,40,0,0,0,CUTTING,COMMON,I_NORMAL_WEAPON,1,WEAPON,"cutlass","cutlass","cutlass" },

{ 94,25,0,0,8,10,0,1,15,50,0,0,0,CUTTING,COMMON,I_NORMAL_WEAPON,1,WEAPON,"hand-axe","hand-axe","hand-axe" },

{ 95,50,0,0,12,8,0,1,15,100,0,0,0,CUTTING,COMMON,I_NORMAL_WEAPON,2,WEAPON,"battle-axe","battle-axe","battle-axe" },

{ 96,100,0,0,24,7,0,1,15,200,0,0,0,CUTTING,COMMON,I_NORMAL_WEAPON,5,WEAPON,"great-axe","great-axe","great-axe" },

{ 97,50,0,0,12,8,0,1,15,50,0,0,0,STRIKING,COMMON,I_NORMAL_WEAPON,2,WEAPON,"mace","mace","mace" },

{ 98,50,0,0,14,6,0,1,15,60,0,0,0,STRIKING,COMMON,I_NORMAL_WEAPON,2,WEAPON,"war-hammer","war-hammer","war-hammer" },

{ 99,60,0,0,16,7,0,1,15,150,0,0,0,STRIKING,COMMON,I_NORMAL_WEAPON,5,WEAPON,"morning-star","morning-star","morning-star" },

{ 100,50,0,0,12,7,0,1,15,50,0,0,0,STRIKING,COMMON,I_NORMAL_WEAPON,3,WEAPON,"flail","flail","flail" },

{ 101,30,0,0,6,10,0,1,15,5,0,0,0,STRIKING,COMMON,I_NORMAL_WEAPON,0,WEAPON,"club","club","club" },

{ 102,80,0,0,9,12,0,1,15,30,0,0,0,STRIKING,COMMON,I_NORMAL_WEAPON,1,WEAPON,"quarterstaff","quarterstaff","quarterstaff" },

{ 103,50,0,0,10,10,0,1,15,50,0,0,0,THRUSTING,COMMON,I_NORMAL_WEAPON,2,WEAPON,"spear","spear","spear" },

{ 104,100,0,0,16,6,0,1,15,100,0,0,0,CUTTING,COMMON,I_NORMAL_WEAPON,3,WEAPON,"halberd","halberd","halberd" },

{ 105,80,0,0,12,7,0,1,15,75,0,0,0,THRUSTING,COMMON,I_NORMAL_WEAPON,3,WEAPON,"trident","trident","trident" },

{ 106,20,0,0,4,8,1005,1,15,100,0,0,0,STRIKING,COMMON,I_NORMAL_WEAPON,2,WEAPON,"whip","whip","whip" },

{ 107,20,0,0,30,20,1002,1,15,2000,0,0,0,THRUSTING,COMMON,I_LIGHTSABRE,9,WEAPON,"grey metal cylinder","lightsabre","lightsabre" },

{ 108,500,8,0,16,8,1003,1,15,1000,0,0,-8,CUTTING,COMMON,I_DEMONBLADE,9,WEAPON,"bastard sword","Demonblade","Demonblade" },

{ 109,250,7,0,17,10,1004,1,15,3000,0,0,7,STRIKING,COMMON,I_MACE_DISRUPT,7,WEAPON,"mace","mace of disruption","mace" },

{ 110,100,0,0,12,15,0,1,15,300,0,0,0,MISSILE,COMMON,I_NORMAL_WEAPON,2,MISSILEWEAPON,"longbow","longbow","longbow" },

{ 111,150,0,0,20,15,0,1,15,500,0,0,0,MISSILE,COMMON,I_NORMAL_WEAPON,3,MISSILEWEAPON,"crossbow","crossbow","crossbow" },

{ 112,2,0,0,3,3,1006,1,15,2,0,0,0,MISSILE,COMMON,I_NORMAL_WEAPON,1,MISSILEWEAPON,"arrow","arrow","arrow" },

{ 113,2,0,0,3,0,1007,1,15,5,0,0,0,MISSILE,COMMON,I_NORMAL_WEAPON,1,MISSILEWEAPON,"bolt","bolt","bolt" },

{ 114,50,0,0,3,10,1005,1,15,50,0,0,0,MISSILE,COMMON,I_NORMAL_WEAPON,2,MISSILEWEAPON,"bola","bola","bola" },

{ 115,40,5,0,12,9,1008,1,15,3000,0,0,0,CUTTING,COMMON,I_NORMAL_WEAPON,7,WEAPON,"broad sword","vorpal sword","sword of self-mutilation" },

{ 116,100,7,0,20,7,1009,1,15,5000,0,0,-7,CUTTING,COMMON,I_DESECRATE,9,WEAPON,"great-sword","Desecrator","Desecrator" },

{ 117,60,7,0,16,7,1010,1,15,2000,0,0,0,STRIKING,COMMON,I_PERM_FIRE_RESIST,6,WEAPON,"morning-star","firestar","firestar" },

{ 118,50,7,0,10,12,1011,1,15,3000,0,0,7,THRUSTING,COMMON,I_DEFEND,7,WEAPON,"estoc","Defender","Defender" },

{ 119,50,10,0,100,10,1012,1,15,5000,0,0,10,THRUSTING,UNIQUE_MADE,I_VICTRIX,10,WEAPON,"ivory spear","Victrix","Victrix" },

{ 120,500,6,0,32,6,0,1,15,2000,0,0,0,CUTTING,UNIQUE_MADE,I_NORMAL_WEAPON,7,WEAPON,"great-axe","Goblins' Hewer","Goblins' Hewer" },

{ 121,100,20,0,30,10,0,1,15,500,0,0,-10,CUTTING,COMMON,I_NORMAL_WEAPON,6,WEAPON,"scythe","scythe of slicing","scythe of slicing" },

{ 122,250,0,0,50,-10,0,1,15,100,0,0,0,STRIKING,COMMON,I_NORMAL_WEAPON,5,WEAPON,"giant club","giant club","giant club" },

{ 123,500,0,0,10000,1000,1014,1,15,10000,0,0,0,CUTTING,UNIQUE_MADE,I_NORMAL_WEAPON,10,WEAPON,"Scythe of Death","Scythe of Death","Scythe of Death" },

{ 124,30,0,0,16,16,1015,1,15,1000,0,0,0,STRIKING,COMMON,I_NORMAL_WEAPON,4,WEAPON,"whip","acid whip","acid whip" },

{ 125,100,0,0,1,0,0,1,20,5,0,0,0,0,COMMON,I_NORMAL_ARMOR,0,ARMOR,"flak jacket","flak jacket","flak jacket" },

{ 126,200,0,0,2,0,1,1,20,10,0,0,0,0,COMMON,I_NORMAL_ARMOR,1,ARMOR,"soft leather armor","soft leather armor","soft leather armor" },

{ 127,200,0,0,2,0,1,1,20,30,0,0,0,0,COMMON,I_NORMAL_ARMOR,1,ARMOR,"cuirbouilli","cuirbouilli","cuirbouilli" },

{ 128,300,0,0,3,0,2,1,20,100,0,0,0,0,COMMON,I_NORMAL_ARMOR,2,ARMOR,"ring mail","ring mail","ring mail" },

{ 129,400,0,0,4,0,3,1,20,200,0,0,0,0,COMMON,I_NORMAL_ARMOR,3,ARMOR,"chain mail","chain mail","chain mail" },

{ 130,400,0,0,4,0,2,1,20,300,0,0,0,0,COMMON,I_NORMAL_ARMOR,4,ARMOR,"scale mail","scale mail","scale mail" },

{ 131,400,0,0,5,0,3,1,20,450,0,0,0,0,COMMON,I_NORMAL_ARMOR,5,ARMOR,"partial plate mail","partial plate mail","partial plate mail" },

{ 132,600,0,0,6,0,4,1,20,750,0,0,0,0,COMMON,I_NORMAL_ARMOR,6,ARMOR,"full plate mail","full plate mail","full plate mail" },

{ 133,500,0,0,7,0,4,1,20,1500,0,0,0,0,COMMON,I_NORMAL_ARMOR,6,ARMOR,"plate armor","plate armor","plate armor" },

{ 134,200,0,0,5,0,2,1,20,1500,0,0,0,0,COMMON,I_NORMAL_ARMOR,6,ARMOR,"lamellar armor","lamellar armor","lamellar armor" },

{ 135,200,0,0,5,0,2,1,20,2000,0,0,0,0,COMMON,I_PERM_FEAR_RESIST,7,ARMOR,"mithril chain mail","mithril chain mail","mithril chain mail" },

{ 136,300,0,0,8,0,3,1,20,3000,0,0,0,0,COMMON,I_PERM_FEAR_RESIST,7,ARMOR,"mithril plate armor","mithril plate armor","mithril plate armor" },

{ 137,500,0,0,6,0,3,1,20,3000,0,0,0,0,COMMON,I_PERM_FIRE_RESIST,7,ARMOR,"dragonscale armor","dragonscale armor","dragonscale armor" },

{ 138,100,9,0,7,0,2,1,20,1000,0,0,0,0,COMMON,I_PERM_ENERGY_RESIST,8,ARMOR,"prismatrix armor","prismatrix armor","prismatrix armor" },

{ 139,0,0,0,20,0,0,1,20,5000,0,0,0,0,COMMON,I_PERM_BREATHING,9,ARMOR,"powered combat armor","powered combat armor","powered combat armor" },

{ 140,0,0,0,10,0,0,1,20,3000,0,0,0,0,COMMON,I_PERM_BREATHING,7,ARMOR,"powered scout armor","powered scout armor","powered scout armor" },

{ 141,100,5,0,5,0,0,1,20,2000,0,0,-9,0,COMMON,I_PERM_ENERGY_RESIST,3,ARMOR,"demonhide armor","demonhide armor","demonhide armor" },

{ 142,20,0,0,0,0,1,1,15,10,0,0,0,0,COMMON,I_NORMAL_SHIELD,0,SHIELD,"buckler","buckler","buckler" },

{ 143,40,0,0,0,0,2,1,15,20,0,0,0,0,COMMON,I_NORMAL_SHIELD,1,SHIELD,"small round shield","small round shield","small round shield" },

{ 144,100,0,0,0,0,3,1,15,50,0,0,0,0,COMMON,I_NORMAL_SHIELD,2,SHIELD,"large round shield","large round shield","large round shield" },

{ 145,200,0,0,0,0,4,1,15,75,0,0,0,0,COMMON,I_NORMAL_SHIELD,2,SHIELD,"heater shield","heater shield","heater shield" },

{ 146,300,0,0,0,0,5,1,15,150,0,0,0,0,COMMON,I_NORMAL_SHIELD,3,SHIELD,"hoplon","hoplon","hoplon" },

{ 147,500,0,0,0,0,6,1,15,200,0,0,0,0,COMMON,I_NORMAL_SHIELD,4,SHIELD,"tower shield","tower shield","tower shield" },

{ 148,20,0,0,0,0,7,1,15,1000,0,0,0,0,COMMON,I_NORMAL_SHIELD,6,SHIELD,"plasteel shield","plasteel shield","plasteel shield" },

{ 149,200,6,0,0,0,6,1,15,2000,0,0,0,0,COMMON,I_PERM_DEFLECT,7,SHIELD,"shield of deflection","shield of deflection","shield of deflection" },

{ 150,30,0,0,0,0,0,1,4,15,0,0,0,0,COMMON,I_NO_OP,0,CLOAK,"?","cloak of wool","cloak of wool" },

{ 151,30,0,0,0,0,0,1,4,500,0,0,0,0,COMMON,I_PERM_NEGIMMUNE,3,CLOAK,"?","cloak of negimmunity","cloak of level drain" },

{ 152,30,0,0,0,0,0,1,4,500,0,0,0,0,COMMON,I_PERM_INVISIBLE,5,CLOAK,"?","cloak of invisibility","cloak of invisibility" },

{ 153,30,0,0,0,0,0,1,4,1000,0,0,0,0,COMMON,I_PERM_ACCURACY,5,CLOAK,"?","cloak of skill","cloak of skill" },

{ 154,30,0,0,0,0,0,1,4,500,0,0,0,0,COMMON,I_PERM_PROTECTION,3,CLOAK,"?","cloak of protection","cloak of damage" },

{ 155,30,0,0,0,0,0,1,4,2000,0,0,0,0,COMMON,I_PERM_DISPLACE,6,CLOAK,"?","cloak of displacement","cloak of vulnerability" },

{ 156,30,0,0,0,0,0,1,4,500,0,0,0,0,COMMON,I_PERM_TRUESIGHT,3,CLOAK,"?","cloak of true sight","cloak of blindness" },

{ 157,30,0,0,0,0,0,1,6,1000,0,0,0,0,COMMON,I_PERM_SPEED,4,BOOTS,"?","boots of speed","boots of slowness" },

{ 158,30,0,0,0,0,0,1,6,1000,0,0,0,0,COMMON,I_PERM_HERO,6,BOOTS,"?","boots of heroism","boots of cowardliness" },

{ 159,30,0,0,0,0,0,1,6,500,0,0,0,0,COMMON,I_PERM_LEVITATE,3,BOOTS,"?","boots of levitation","boots of levitation" },

{ 160,30,0,0,0,0,0,1,6,250,0,0,0,0,COMMON,I_PERM_AGILITY,3,BOOTS,"?","boots of agility","boots of clumsiness" },

{ 161,30,0,0,0,0,0,1,6,200,0,0,0,0,COMMON,I_BOOTS_JUMPING,2,BOOTS,"?","jumping boots","jumping boots" },

{ 162,30,0,0,0,0,0,1,6,7,0,0,0,0,COMMON,I_NO_OP,0,BOOTS,"?","boots of leather","boots of leather" },

{ 163,30,0,0,0,0,0,1,6,2700,0,0,0,0,COMMON,I_BOOTS_7LEAGUE,7,BOOTS,"?","seven league boots","umpteen league boots" },

{ 164,1,0,0,0,0,0,1,10,400,0,0,0,0,COMMON,I_PERM_TRUESIGHT,3,RING,"?","ring of truesight","ring of blindness" },

{ 165,1,0,0,0,0,0,1,10,1,0,0,-1,0,COMMON,I_PERM_BURDEN,0,RING,"?","ring of burden","ring of burden" },

{ 166,1,0,0,0,0,0,1,10,100,0,0,0,0,COMMON,I_PERM_STRENGTH,1,RING,"?","ring of strength","ring of strength" },

{ 167,1,0,0,0,0,0,1,10,100,0,0,0,0,COMMON,I_PERM_GAZE_IMMUNE,2,RING,"?","ring of gaze immunity","ring of gaze immunity" },

{ 168,1,0,0,0,0,0,1,10,100,0,0,0,0,COMMON,I_PERM_FIRE_RESIST,3,RING,"?","ring of fire resistance","ring of fire resistance" },

{ 169,1,0,0,0,0,0,1,10,10,0,0,0,0,COMMON,I_NO_OP,0,RING,"?","ring of brass and glass","ring of brass and glass" },

{ 170,1,0,0,0,0,0,1,10,100,0,0,0,0,COMMON,I_PERM_POISON_RESIST,4,RING,"?","ring of poison resistance","ring of poison" },

{ 171,1,0,0,0,0,0,1,10,1000,0,0,0,0,COMMON,I_PERM_REGENERATE,5,RING,"?","ring of regeneration","ring of regeneration" },

{ 172,1,0,0,0,0,0,1,10,100,0,0,0,0,COMMON,I_PERM_KNOWLEDGE,0,RING,"?","ring of self-knowledge","ring of delusion" },

{ 173,1,0,0,0,0,0,1,10,500,0,0,0,0,COMMON,I_PERM_PROTECTION,4,RING,"?","ring of protection","ring of vulnerability" },

{ 174,80,0,0,0,0,0,1,10,500,0,0,0,0,COMMON,I_FIREBOLT,3,STICK,"?","staff of firebolts","staff of firebolts" },

{ 175,80,0,0,0,0,0,1,10,10,0,0,0,0,COMMON,I_NOTHING,0,STICK,"?","walking stick","walking stick" },

{ 176,80,0,0,0,0,0,1,10,100,0,0,0,0,COMMON,I_SLEEP_OTHER,1,STICK,"?","staff of sleep","staff of sleep" },

{ 177,80,0,0,0,0,0,1,10,500,0,0,0,0,COMMON,I_LBALL,4,STICK,"?","wand of ball lightning","wand of ball lightning" },

{ 178,80,0,0,0,0,0,1,10,500,0,0,0,0,COMMON,I_SUMMON,2,STICK,"?","rod of summoning","rod of summoning" },

{ 179,80,0,0,0,0,0,1,10,100,0,0,0,0,COMMON,I_HIDE,1,STICK,"?","rod of hiding","rod of hiding" },

{ 180,80,0,0,0,0,0,1,10,500,0,0,0,0,COMMON,I_LBOLT,3,STICK,"?","staff of lightning bolts","staff of lightning bolts" },

{ 181,80,0,0,0,0,0,1,10,500,0,0,0,0,COMMON,I_FIREBALL,5,STICK,"?","wand of fireballs","wand of fireballs" },

{ 182,80,0,0,0,0,0,1,10,2000,0,0,0,0,COMMON,I_DISINTEGRATE,7,STICK,"?","rod of disintegration","rod of disintegration" },

{ 183,80,0,0,0,0,0,1,10,1000,0,0,0,0,COMMON,I_DISRUPT,6,STICK,"?","staff of disruption","staff of disruption" },

{ 184,80,0,0,0,0,0,1,10,100,0,0,0,0,COMMON,I_SNOWBALL,2,STICK,"?","snowball stick","snowball stick" },

{ 185,80,0,0,0,0,0,1,10,50,0,0,0,0,COMMON,I_MISSILE,1,STICK,"?","staff of missiles","staff of missiles" },

{ 186,80,0,0,0,0,0,1,10,200,0,0,0,0,COMMON,I_APPORT,2,STICK,"?","rod of apportation","rod of lossage" },

{ 187,80,0,0,0,0,0,1,10,750,0,0,0,0,COMMON,I_DISPEL,6,STICK,"?","staff of dispelling","staff of self-dispelling" },

{ 188,80,0,0,0,0,0,1,10,500,0,0,0,0,COMMON,I_HEAL,3,STICK,"?","staff of healing","staff of harming" },

{ 189,80,0,0,0,0,0,1,10,1000,0,0,0,0,COMMON,I_POLYMORPH,3,STICK,"wand of stupidity","wand of polymorph","wand of stupidity" },

{ 190,80,0,0,0,0,0,1,10,500,0,0,0,0,COMMON,I_FEAR,2,STICK,"?","wand of fear","wand of fear" },

{ 191,500,0,0,0,0,0,1,15,10000,0,0,0,0,UNIQUE_MADE,I_ORBMASTERY,10,ARTIFACT,"Mysterious Orb","Orb of Mastery","Orb of Mastery" },

{ 192,50,0,0,0,0,0,1,15,2000,0,0,0,0,COMMON,I_ORBFIRE,9,ARTIFACT,"Mysterious Orb","Orb of Fire","Orb of Fire" },

{ 193,50,0,0,0,0,0,1,15,2000,0,0,0,0,COMMON,I_ORBWATER,8,ARTIFACT,"Mysterious Orb","Orb of Water","Orb of Water" },

{ 194,50,0,0,0,0,0,1,15,2000,0,0,0,0,COMMON,I_ORBEARTH,7,ARTIFACT,"Mysterious Orb","Orb of Earth","Orb of Earth" },

{ 195,50,0,0,0,0,0,1,15,2000,0,0,0,0,COMMON,I_ORBAIR,6,ARTIFACT,"Mysterious Orb","Orb of Air","Orb of Air" },

{ 196,25,0,0,0,0,0,1,15,100,0,0,0,0,COMMON,I_ORBDEAD,1,ARTIFACT,"Black Orb","Burnt-out Orb","Burnt-out Orb" },

{ 197,75,0,0,0,0,0,1,15,500,0,0,0,0,COMMON,I_CRYSTAL,3,ARTIFACT,"Glass Orb","Crystal Ball","Ball of Mindlessness" },

{ 198,25,0,0,0,0,0,1,15,1000,0,0,0,0,COMMON,I_ANTIOCH,5,ARTIFACT,"Metal Pineapple","Holy Hand-Grenade of Antioch","Holy Hand-Grenade of Antioch" },

{ 199,500,0,0,0,0,0,1,15,1,0,0,0,0,UNIQUE_UNMADE,I_NOTHING,1,ARTIFACT,"Mystic Amulet","Amulet of Yendor","Amulet of Yendor" },

{ 200,1,0,0,0,0,0,1,15,20000,0,0,0,0,UNIQUE_MADE,I_KOLWYNIA,7,ARTIFACT,"Key","Kolwynia, the Key That Was Lost","Kolwynia, the Key That was Lost" },

{ 201,10,0,0,0,0,0,1,15,500,0,0,0,0,UNIQUE_UNMADE,I_DEATH,3,ARTIFACT,"?","Potion of Death","Potion of Death" },

{ 202,100,0,0,0,0,0,1,15,2000,0,0,0,0,UNIQUE_UNMADE,I_ENCHANTMENT,5,ARTIFACT,"Scintillating Staff","Staff of Enchantment","Staff of Enchantment" },

{ 203,0,0,0,0,0,0,1,15,1500,0,0,0,0,UNIQUE_UNMADE,I_HELM,2,ARTIFACT,"Strange Weightless Helm","Helm of Teleportation","Helm of Teleportation" },

{ 204,10,0,0,0,0,0,1,15,500,0,0,0,0,UNIQUE_UNMADE,I_LIFE,6,ARTIFACT,"?","Potion of Life","Potion of Life" },

{ 205,5,0,0,0,0,1,1,15,5000,0,0,0,0,UNIQUE_MADE,I_SYMBOL,10,ARTIFACT,"Silver Gallows","Holy Symbol of Odin","Holy Symbol of Odin" },

{ 206,5,0,0,0,0,2,1,15,5000,0,0,0,0,UNIQUE_MADE,I_SYMBOL,10,ARTIFACT,"Jet Scarab","Holy Symbol of Set","Holy Symbol of Set" },

{ 207,5,0,0,0,0,3,1,15,5000,0,0,0,0,UNIQUE_MADE,I_SYMBOL,10,ARTIFACT,"Gold Owl Pendant","Holy Symbol of Athena","Holy Symbol of Athena" },

{ 208,5,0,0,0,0,4,1,15,5000,0,0,0,0,UNIQUE_MADE,I_SYMBOL,10,ARTIFACT,"Obsidian Crescent","Holy Symbol of Hecate","Holy Symbol of Hecate" },

{ 209,5,0,0,0,0,5,1,15,5000,0,0,0,0,UNIQUE_MADE,I_SYMBOL,10,ARTIFACT,"Sprig of Mistletoe","Holy Symbol of Druidism","Holy Symbol of Druidism" },

{ 210,5,0,0,0,0,6,1,15,5000,0,0,0,0,UNIQUE_MADE,I_SYMBOL,10,ARTIFACT,"Blue Crystal Pendant","Holy Symbol of Destiny","Holy Symbol of Destiny" },

{ 211,1000,0,0,0,0,0,1,15,1000,0,0,0,0,UNIQUE_UNMADE,I_JUGGERNAUT,6,ARTIFACT,"Crudely Carved Monolith","Juggernaut of Karnak","Juggernaut of Karnak" },

{ 212,10,0,0,0,0,0,1,0,10000,0,0,0,0,UNIQUE_MADE,I_STARGEM,7,ARTIFACT,"Strangely Glowing Crystal","Star Gem","Star Gem" },

{ 213,1000,0,0,0,0,0,1,10,1000,0,0,0,0,UNIQUE_MADE,I_SCEPTRE,10,ARTIFACT,"Extremely Heavy Stick ","Sceptre of High Magic","Sceptre of High Magic" },

{ 214,10,0,0,0,0,0,1,10,1000,0,0,0,0,UNIQUE_MADE,I_PLANES,10,ARTIFACT,"Octagonal Copper Amulet ","Amulet of the Planes","Amulet of the Planes" },

{ 215,0,0,0,0,0,0,1,10,0,0,0,0,0,COMMON,I_NO_OP,0,CASH,"money","money","money (the root of all evil)" },

{ 216,100,0,0,0,0,0,1,10,0,0,0,0,0,COMMON,I_CORPSE,0,CORPSE,"a mysterious corpse","a mysterious corpse","a mysterious corpse" }
};
