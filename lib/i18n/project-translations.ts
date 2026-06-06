/**
 * Project Translations - All 28 RAMS Projects
 * Languages: Russian (ru), Kazakh (kk), Turkish (tr), English (en)
 * FULL ORIGINAL TEXTS from CSV
 */

import { Language } from "./translations";

export interface LocalizedText {
  ru: string;
  kk: string;
  tr: string;
  en: string;
}

export interface ProjectTranslation {
  title: LocalizedText;
  subtitle: LocalizedText;
  description: LocalizedText;
  status: LocalizedText;
  features: LocalizedText[];
}

// Status translations
export const statusTranslations: Record<string, LocalizedText> = {
  "Строится": { ru: "Строится", kk: "Салынуда", tr: "İnşaat Halinde", en: "Under Construction" },
  "Сдан": { ru: "Сдан", kk: "Тапсырылды", tr: "Tamamlandı", en: "Completed" },
  "Сдана 1 очередь": { ru: "Сдана 1 очередь", kk: "1-кезең тапсырылды", tr: "1. Etap Tamamlandı", en: "Phase 1 Completed" },
  "Завершен": { ru: "Завершен", kk: "Аяқталды", tr: "Tamamlandı", en: "Completed" },
};

// Scene type translations
export const sceneTypeTranslations: Record<string, LocalizedText> = {
  "Видео": { ru: "Видео", kk: "Бейне", tr: "Video", en: "Video" },
  "Фото": { ru: "Фото", kk: "Фото", tr: "Fotoğraf", en: "Photo" },
};

// Scene title translations
export const sceneTitleTranslations: Record<string, LocalizedText> = {
  "Презентация": { ru: "Презентация", kk: "Презентация", tr: "Sunum", en: "Presentation" },
  "Главный вид": { ru: "Главный вид", kk: "Негізгі көрініс", tr: "Ana Görünüm", en: "Main View" },
};

// Quarter translations
export const quarterTranslations: Record<string, LocalizedText> = {
  "Завершен": { ru: "Завершен", kk: "Аяқталды", tr: "Tamamlandı", en: "Completed" },
  "I": { ru: "I квартал", kk: "I тоқсан", tr: "1. Çeyrek", en: "Q1" },
  "II": { ru: "II квартал", kk: "II тоқсан", tr: "2. Çeyrek", en: "Q2" },
  "III": { ru: "III квартал", kk: "III тоқсан", tr: "3. Çeyrek", en: "Q3" },
  "IV": { ru: "IV квартал", kk: "IV тоқсан", tr: "4. Çeyrek", en: "Q4" },
};

// Location label translations (common ones)
export const locationTranslations: Record<string, LocalizedText> = {
  "Центр": { ru: "Центр", kk: "Орталық", tr: "Merkez", en: "Center" },
  "Центр Атырау": { ru: "Центр Атырау", kk: "Атырау орталығы", tr: "Atırau Merkezi", en: "Atyrau Center" },
  "Астана": { ru: "Астана", kk: "Астана", tr: "Astana", en: "Astana" },
  "Индустриальная зона": { ru: "Индустриальная зона", kk: "Өнеркәсіптік аймақ", tr: "Sanayi Bölgesi", en: "Industrial Zone" },
  "Центральный парк": { ru: "Центральный парк", kk: "Орталық саябақ", tr: "Merkez Park", en: "Central Park" },
  "Алмалинский район": { ru: "Алмалинский район", kk: "Алмалы ауданы", tr: "Almalı Bölgesi", en: "Almaly District" },
  "Бостандыкский район": { ru: "Бостандыкский район", kk: "Бостандық ауданы", tr: "Bostanlık Bölgesi", en: "Bostandyk District" },
  "RAMS City": { ru: "RAMS City", kk: "RAMS City", tr: "RAMS City", en: "RAMS City" },
  "Ерменсай": { ru: "Ерменсай", kk: "Ерменсай", tr: "Yermensay", en: "Yermensay" },
  "Ремизовка": { ru: "Ремизовка", kk: "Ремизовка", tr: "Remizovka", en: "Remizovka" },
};

// Unit translations for measurements
export const unitTranslations: Record<Language, { meters: string; km: string }> = {
  ru: { meters: "м", km: "км" },
  kk: { meters: "м", km: "км" },
  tr: { meters: "m", km: "km" },
  en: { meters: "m", km: "km" },
};

// Property class translations
export const classTranslations: Record<string, LocalizedText> = {
  "Premium+": { ru: "Premium+", kk: "Premium+", tr: "Premium+", en: "Premium+" },
  "Premium": { ru: "Premium", kk: "Premium", tr: "Premium", en: "Premium" },
  "Business+": { ru: "Business+", kk: "Business+", tr: "Business+", en: "Business+" },
  "Business": { ru: "Business", kk: "Business", tr: "Business", en: "Business" },
  "Business A+": { ru: "Бизнес А+", kk: "Бизнес А+", tr: "Business A+", en: "Business A+" },
  "Comfort+": { ru: "Комфорт+", kk: "Комфорт+", tr: "Konfor+", en: "Comfort+" },
  "Comfort": { ru: "Комфорт", kk: "Комфорт", tr: "Konfor", en: "Comfort" },
  "Elite": { ru: "Элит", kk: "Элит", tr: "Elit", en: "Elite" },
  "Resort": { ru: "Курорт", kk: "Курорт", tr: "Tatil", en: "Resort" },
  "Культурный": { ru: "Культурный", kk: "Мәдени", tr: "Kültürel", en: "Cultural" },
  "Промышленный": { ru: "Промышленный", kk: "Өнеркәсіптік", tr: "Endüstriyel", en: "Industrial" },
  "Образовательный": { ru: "Образовательный", kk: "Білім беру", tr: "Eğitim", en: "Educational" },
};

export const projectTranslations: Record<string, ProjectTranslation> = {
  // 1. RAMS BEYOND
  "01-rams-beyond-st-regis": {
    title: { ru: "ST. REGIS ALMATY", kk: "ST. REGIS ALMATY", tr: "ST. REGIS ALMATI", en: "ST. REGIS ALMATY" },
    subtitle: { ru: "Архитектурный ансамбль", kk: "Сәулет ансамблі", tr: "Mimari Topluluk", en: "Architectural Ensemble" },
    description: {
      ru: "RAMS Beyond Almaty — амбициозный проект, способный стать отражением амбиций города, новой точкой притяжения и визитной карточкой для жителей, гостей и инвесторов. Он будет работать на имидж Алматы и усиливать его статус на десятилетия вперёд.",
      kk: "RAMS Beyond Almaty — қаланың амбицияларын көрсете алатын, тұрғындар, қонақтар мен инвесторлар үшін жаңа тартымдылық орталығы және визит картасы бола алатын амбициялық жоба. Ол Алматының имиджіне жұмыс істейді және оның мәртебесін ондаған жылдар бойы нығайтады.",
      tr: "RAMS Beyond Almaty — şehrin tutkularını yansıtabilecek, sakinler, misafirler ve yatırımcılar için yeni bir cazibe merkezi ve kartvizit olabilecek iddialı bir proje. Onlarca yıl boyunca Almatı'nın imajını güçlendirecek.",
      en: "RAMS Beyond Almaty — an ambitious project that can reflect the city's ambitions, becoming a new attraction point and business card for residents, guests, and investors. It will work on the image of Almaty and strengthen its status for decades to come.",
    },
    status: { ru: "Строится", kk: "Салынуда", tr: "İnşaat Halinde", en: "Under Construction" },
    features: [
      { ru: "Архитектурный ансамбль из 8 зданий, объединяющий ключевые сферы современной городской жизни", kk: "Қазіргі қала өмірінің негізгі салаларын біріктіретін 8 ғимараттан тұратын сәулет ансамблі", tr: "Modern şehir yaşamının temel alanlarını birleştiren 8 binadan oluşan mimari topluluk", en: "Architectural ensemble of 8 buildings uniting key areas of modern urban life" },
      { ru: "Beyond Street Mall — торговые площади для мировых брендов и люксовых бутиков в формате современной торговой улицы", kk: "Beyond Street Mall — заманауи сауда көшесі форматындағы әлемдік брендтер мен люкс бутиктерге арналған сауда алаңдары", tr: "Beyond Street Mall — modern ticaret caddesi formatında dünya markaları ve lüks butikler için ticaret alanları", en: "Beyond Street Mall — retail spaces for world brands and luxury boutiques in a modern shopping street format" },
      { ru: "Beyond Gourmet Gallery — рестораны высокой кухни, авторские гастробары и концепты от ведущих шеф-поваров", kk: "Beyond Gourmet Gallery — жоғары асхана мейрамханалары, авторлық гастробарлар және жетекші аспазшылардың концептілері", tr: "Beyond Gourmet Gallery — üst düzey restoranlar, imza gastrobarlar ve önde gelen şeflerden konseptler", en: "Beyond Gourmet Gallery — fine dining restaurants, signature gastrobars and concepts from leading chefs" },
      { ru: "Beyond Office Spaces — офисы класса А+ для международных корпораций и лидеров казахстанского бизнеса", kk: "Beyond Office Spaces — халықаралық корпорациялар мен қазақстандық бизнес көшбасшылары үшін А+ класты кеңселер", tr: "Beyond Office Spaces — uluslararası şirketler ve Kazakistan iş liderleri için A+ sınıfı ofisler", en: "Beyond Office Spaces — Class A+ offices for international corporations and leaders of Kazakhstani business" },
      { ru: "Beyond Residences — коллекция премиальных резиденций", kk: "Beyond Residences — премиум резиденциялар коллекциясы", tr: "Beyond Residences — premium konutlar koleksiyonu", en: "Beyond Residences — collection of premium residences" },
      { ru: "Башня — новый символ города: первый в Алматы отель St. Regis", kk: "Мұнара — қаланың жаңа символы: Алматыдағы алғашқы St. Regis қонақ үйі", tr: "Kule — şehrin yeni sembolü: Almatı'daki ilk St. Regis oteli", en: "Tower — the new symbol of the city: the first St. Regis hotel in Almaty" },
      { ru: "Смотровая площадка с видом на Алматы и Заилийский Алатау", kk: "Алматы мен Іле Алатауына қарайтын шолу алаңы", tr: "Almatı ve Zailiysky Alatau manzaralı seyir terası", en: "Observation deck with views of Almaty and Zailiysky Alatau" },
      { ru: "Конгресс-центр — отдельное здание для масштабных мероприятий: банкетные залы (Ballroom St. Regis), современный конференц-зал для международных событий", kk: "Конгресс-орталық — ауқымды іс-шаралар үшін жеке ғимарат: банкет залдары (Ballroom St. Regis), халықаралық оқиғаларға арналған заманауи конференц-зал", tr: "Kongre merkezi — büyük etkinlikler için ayrı bina: ziyafet salonları (Ballroom St. Regis), uluslararası etkinlikler için modern konferans salonu", en: "Congress center — separate building for large-scale events: banquet halls (Ballroom St. Regis), modern conference hall for international events" },
      { ru: "Beyond Art Hub Almaty — выставки мирового уровня, экспозиции современных казахстанских художников и арт-инсталляции", kk: "Beyond Art Hub Almaty — әлемдік деңгейдегі көрмелер, қазіргі қазақстандық суретшілердің экспозициялары және арт-инсталляциялар", tr: "Beyond Art Hub Almaty — dünya çapında sergiler, çağdaş Kazak sanatçıların sergileri ve sanat enstalasyonları", en: "Beyond Art Hub Almaty — world-class exhibitions, expositions of contemporary Kazakhstani artists and art installations" },
    ],
  },

  // 2. RAMS CITY
  "02-rams-city-kazakhstan": {
    title: { ru: "KAZAKHSTAN", kk: "KAZAKHSTAN", tr: "KAZAKHSTAN", en: "KAZAKHSTAN" },
    subtitle: { ru: "Европейский стиль", kk: "Еуропалық стиль", tr: "Avrupa tarzı", en: "European style" },
    description: {
      ru: "Rams City – современный жилой комплекс в европейском стиле. Проживать здесь будет комфортно всем, но в первую очередь семьям с детьми: именно под таких жильцов «заточена» вся внутренняя инфраструктура ЖК. Особое преимущество квартир в комплексе – из окон открывается шикарный панорамный вид.\n\nВ жилом комплексе RAMS CITY мы подготовили для вас множество коммерческих помещений, которые будут расположены вдоль нашей грандиозной аллеи. Ваш бизнес будет находиться в центре событий.",
      kk: "Rams City – еуропалық стильдегі заманауи тұрғын үй кешені. Мұнда бәріне жайлы болады, бірақ ең алдымен балалы отбасыларға: ТҮК-нің барлық ішкі инфрақұрылымы осындай тұрғындарға арналған. Кешендегі пәтерлердің ерекше артықшылығы – терезелерден керемет панорамалық көрініс ашылады.\n\nRAMS CITY тұрғын үй кешенінде біз сіздер үшін біздің керемет аллея бойында орналасатын көптеген коммерциялық үй-жайлар дайындадық. Сіздің бизнесіңіз оқиғалардың орталығында болады.",
      tr: "Rams City – Avrupa tarzında modern bir konut kompleksi. Burada herkes için rahat olacak, ancak öncelikle çocuklu aileler için: konut kompleksinin tüm iç altyapısı bu tür sakinler için tasarlanmıştır. Kompleksteki dairelerin özel avantajı – pencerelerden muhteşem panoramik manzara açılıyor.\n\nRAMS CITY konut kompleksinde, görkemli bulvarımız boyunca yer alacak birçok ticari alan hazırladık. İşletmeniz olayların merkezinde olacak.",
      en: "Rams City – a modern residential complex in European style. Living here will be comfortable for everyone, but primarily for families with children: all internal infrastructure of the complex is designed for such residents. The special advantage of apartments in the complex – stunning panoramic views open from the windows.\n\nIn the RAMS CITY residential complex, we have prepared many commercial spaces for you, which will be located along our grand boulevard. Your business will be at the center of events.",
    },
    status: { ru: "Сдана 1 очередь", kk: "1-кезең тапсырылды", tr: "1. Etap Tamamlandı", en: "Phase 1 Completed" },
    features: [
      { ru: "Отличная локация в сердце города, у реки Большая Алматинка", kk: "Үлкен Алматы өзенінің жанында, қала жүрегіндегі тамаша орналасу", tr: "Büyük Almatinka nehri yakınında, şehrin kalbinde mükemmel konum", en: "Excellent location in the heart of the city, near the Bolshaya Almatinka river" },
      { ru: "Школа во дворе", kk: "Ауладағы мектеп", tr: "Avluda okul", en: "School in the courtyard" },
      { ru: "Близость к самым большим ТРЦ Алматы – ADK и Mega", kk: "Алматының ең үлкен СОО-на жақындық – ADK және Mega", tr: "Almatı'nın en büyük AVM'lerine yakınlık – ADK ve Mega", en: "Proximity to the largest shopping malls in Almaty – ADK and Mega" },
      { ru: "В пяти минутах Парк Первого Президента", kk: "Бес минутта Бірінші Президент паркі", tr: "Beş dakikada Birinci Cumhurbaşkanı Parkı", en: "Five minutes to the First President Park" },
      { ru: "Зеленый RAMS бульвар протяженностью более километра с развитой сетью стрит-ритейл", kk: "Дамыған стрит-ритейл желісі бар бір километрден астам RAMS жасыл бульвары", tr: "Gelişmiş sokak perakende ağı ile bir kilometreden uzun yeşil RAMS bulvarı", en: "Green RAMS boulevard over one kilometer long with developed street-retail network" },
      { ru: "Современные детские и workout – площадки и места для отдыха всех домочадцев", kk: "Барлық үй тұрғындары үшін заманауи балалар және воркаут алаңдары мен демалыс орындары", tr: "Tüm ev halkı için modern çocuk ve spor alanları ve dinlenme yerleri", en: "Modern children's and workout areas and recreation places for all household members" },
      { ru: "Комнаты развлечений для взрослых и детей", kk: "Ересектер мен балаларға арналған ойын-сауық бөлмелері", tr: "Yetişkinler ve çocuklar için eğlence odaları", en: "Entertainment rooms for adults and children" },
      { ru: "Фонтан, как место встреч", kk: "Кездесу орны ретіндегі фонтан", tr: "Buluşma yeri olarak çeşme", en: "Fountain as a meeting place" },
    ],
  },

  // 3. NOMAD
  "03-nomad": {
    title: { ru: "NOMAD", kk: "NOMAD", tr: "NOMAD", en: "NOMAD" },
    subtitle: { ru: "Современный стиль", kk: "Заманауи стиль", tr: "Modern tarz", en: "Modern style" },
    description: {
      ru: "Современный жилой комплекс Nomad располагается в тихом и уютном Алмалинском районе неподалеку от улиц Толе би и Жарокова.\n\nДворы расположены на стилобате и оборудованы зонами отдыха, игровыми площадками с качелями и горками, спортивными площадками. Предусмотрен подземный паркинг на 352 места и гостевой – на 70 автомобилей.\n\nПо соседству с комплексом есть два торговых центра, несколько разнообразных магазинов, сетевые супермаркеты, банковское отделение. Через дорогу от новостройки находятся кафе и модные рестораны, кофейни, заведение быстрого питания, салон красоты.",
      kk: "Nomad заманауи тұрғын үй кешені Төле би және Жароков көшелерінен алыс емес тыныш және жайлы Алмалы ауданында орналасқан.\n\nАулалар стилобатта орналасқан және демалыс аймақтарымен, әткеншектер мен сырғанақтары бар ойын алаңдарымен, спорт алаңдарымен жабдықталған. 352 орынға арналған жер асты паркингі және 70 автомобильге арналған қонақ паркингі қарастырылған.\n\nКешеннің жанында екі сауда орталығы, бірнеше түрлі дүкендер, желілік супермаркеттер, банк бөлімшесі бар. Жаңа ғимараттан жолдың арғы жағында кафелер мен сәнді мейрамханалар, кофеханалар, тез тамақтану орны, сұлулық салоны орналасқан.",
      tr: "Modern Nomad konut kompleksi, Tole bi ve Jarokov sokaklarına yakın sessiz ve rahat Almalinsky bölgesinde yer almaktadır.\n\nAvlular stilobat üzerinde yer almakta olup dinlenme alanları, salıncaklar ve kaydıraklarla donatılmış oyun alanları ve spor alanlarıyla donatılmıştır. 352 araçlık yeraltı otoparkı ve 70 araçlık misafir otoparkı bulunmaktadır.\n\nKompleksin yanında iki alışveriş merkezi, çeşitli mağazalar, zincir süpermarketler ve banka şubesi bulunmaktadır. Yeni binadan yolun karşısında kafeler ve şık restoranlar, kahve dükkanları, fast food mekanları ve güzellik salonu yer almaktadır.",
      en: "The modern Nomad residential complex is located in the quiet and cozy Almalinsky district not far from Tole bi and Zharokov streets.\n\nThe courtyards are located on a stylobate and equipped with recreation areas, playgrounds with swings and slides, and sports grounds. Underground parking for 352 cars and guest parking for 70 cars are provided.\n\nNext to the complex there are two shopping centers, several various stores, chain supermarkets, and a bank branch. Across the road from the new building there are cafes and fashionable restaurants, coffee shops, fast food establishments, and a beauty salon.",
    },
    status: { ru: "Сдан", kk: "Тапсырылды", tr: "Tamamlandı", en: "Completed" },
    features: [
      { ru: "Стадионы и спортивные площадки по близости", kk: "Жақын жердегі стадиондар мен спорт алаңдары", tr: "Yakınlarda stadyumlar ve spor alanları", en: "Stadiums and sports grounds nearby" },
      { ru: "Развитая инфраструктура", kk: "Дамыған инфрақұрылым", tr: "Gelişmiş altyapı", en: "Developed infrastructure" },
      { ru: "Лучшее предложение в своем сегменте", kk: "Өз сегментіндегі ең жақсы ұсыныс", tr: "Segmentindeki en iyi teklif", en: "Best offer in its segment" },
      { ru: "Центр города", kk: "Қала орталығы", tr: "Şehir merkezi", en: "City center" },
      { ru: "Детский сад на 200 мест на территории ЖК", kk: "ТҮК аумағында 200 орындық балабақша", tr: "Konut kompleksi alanında 200 kişilik anaokulu", en: "Kindergarten for 200 children on the complex territory" },
      { ru: "Уютный двор", kk: "Жайлы аула", tr: "Rahat avlu", en: "Cozy courtyard" },
      { ru: "Большой подземный паркинг на 352 места", kk: "352 орынға арналған үлкен жер асты паркингі", tr: "352 araçlık büyük yeraltı otoparkı", en: "Large underground parking for 352 cars" },
      { ru: "Современный дизайн", kk: "Заманауи дизайн", tr: "Modern tasarım", en: "Modern design" },
      { ru: "Идеально подходящие семейные планировки", kk: "Отбасылық жоспарларға өте қолайлы", tr: "Aileler için ideal planlar", en: "Ideally suitable family layouts" },
      { ru: "Сейсмостойкость", kk: "Сейсмикалық төзімділік", tr: "Depreme dayanıklılık", en: "Seismic resistance" },
    ],
  },

  // 4. NOMAD 2
  "04-nomad-2": {
    title: { ru: "NOMAD 2", kk: "NOMAD 2", tr: "NOMAD 2", en: "NOMAD 2" },
    subtitle: { ru: "Продолжение успеха", kk: "Табыстың жалғасы", tr: "Başarının devamı", en: "Continuation of success" },
    description: {
      ru: "Современный жилой комплекс Nomad 2 располагается в тихом и уютном Алмалинском районе от улиц Толе би и Гагарина.\n\nДворы расположены на стилобате и оборудованы зонами отдыха, игровыми площадками с качелями и горками, спортивными площадками.\n\nПо соседству с комплексом есть два торговых центра, несколько разнообразных магазинов, сетевые супермаркеты, банковское отделение. Через дорогу от новостройки находятся кафе и модные рестораны, кофейни, заведение быстрого питания, салон красоты.",
      kk: "Nomad 2 заманауи тұрғын үй кешені Төле би және Гагарин көшелерінен алыс емес тыныш және жайлы Алмалы ауданында орналасқан.\n\nАулалар стилобатта орналасқан және демалыс аймақтарымен, әткеншектер мен сырғанақтары бар ойын алаңдарымен, спорт алаңдарымен жабдықталған.\n\nКешеннің жанында екі сауда орталығы, бірнеше түрлі дүкендер, желілік супермаркеттер, банк бөлімшесі бар. Жаңа ғимараттан жолдың арғы жағында кафелер мен сәнді мейрамханалар, кофеханалар, тез тамақтану орны, сұлулық салоны орналасқан.",
      tr: "Modern Nomad 2 konut kompleksi, Tole bi ve Gagarin sokaklarına yakın sessiz ve rahat Almalinsky bölgesinde yer almaktadır.\n\nAvlular stilobat üzerinde yer almakta olup dinlenme alanları, salıncaklar ve kaydıraklarla donatılmış oyun alanları ve spor alanlarıyla donatılmıştır.\n\nKompleksin yanında iki alışveriş merkezi, çeşitli mağazalar, zincir süpermarketler ve banka şubesi bulunmaktadır. Yeni binadan yolun karşısında kafeler ve şık restoranlar, kahve dükkanları, fast food mekanları ve güzellik salonu yer almaktadır.",
      en: "The modern Nomad 2 residential complex is located in the quiet and cozy Almalinsky district not far from Tole bi and Gagarin streets.\n\nThe courtyards are located on a stylobate and equipped with recreation areas, playgrounds with swings and slides, and sports grounds.\n\nNext to the complex there are two shopping centers, several various stores, chain supermarkets, and a bank branch. Across the road from the new building there are cafes and fashionable restaurants, coffee shops, fast food establishments, and a beauty salon.",
    },
    status: { ru: "Сдан", kk: "Тапсырылды", tr: "Tamamlandı", en: "Completed" },
    features: [
      { ru: "Стадионы и спортивные площадки по близости", kk: "Жақын жердегі стадиондар мен спорт алаңдары", tr: "Yakınlarda stadyumlar ve spor alanları", en: "Stadiums and sports grounds nearby" },
      { ru: "Развитая инфраструктура", kk: "Дамыған инфрақұрылым", tr: "Gelişmiş altyapı", en: "Developed infrastructure" },
      { ru: "Лучшее предложение в своем сегменте", kk: "Өз сегментіндегі ең жақсы ұсыныс", tr: "Segmentindeki en iyi teklif", en: "Best offer in its segment" },
      { ru: "Центр города", kk: "Қала орталығы", tr: "Şehir merkezi", en: "City center" },
      { ru: "Детский сад на 200 мест на территории ЖК", kk: "ТҮК аумағында 200 орындық балабақша", tr: "Konut kompleksi alanında 200 kişilik anaokulu", en: "Kindergarten for 200 children on the complex territory" },
      { ru: "Уютный двор", kk: "Жайлы аула", tr: "Rahat avlu", en: "Cozy courtyard" },
      { ru: "Большой подземный паркинг", kk: "Үлкен жер асты паркингі", tr: "Büyük yeraltı otoparkı", en: "Large underground parking" },
      { ru: "Современный дизайн", kk: "Заманауи дизайн", tr: "Modern tasarım", en: "Modern design" },
      { ru: "Квартиры площадью от 42 до 104 м²", kk: "42-ден 104 м²-ге дейінгі пәтерлер", tr: "42 ile 104 m² arası daireler", en: "Apartments from 42 to 104 m²" },
      { ru: "Сейсмостойкость", kk: "Сейсмикалық төзімділік", tr: "Depreme dayanıklılık", en: "Seismic resistance" },
    ],
  },

  // 5. ДОМ НА АБАЯ
  "05-dom-na-abaya": {
    title: { ru: "ДОМ НА АБАЯ", kk: "ДОМ НА АБАЯ", tr: "ДОМ НА АБАЯ", en: "ДОМ НА АБАЯ" },
    subtitle: { ru: "Престижная локация", kk: "Беделді орналасу", tr: "Prestijli konum", en: "Prestigious location" },
    description: {
      ru: "ЖК «Дом на Абая» — уют и спокойствие в центре мегаполиса!\n\nЖилой комплекс «Дом на Абая» находится между проспектом Гагарина и улицей Айманова, вблизи пересечения с проспектом Абая. Рядом расположена станция метро «Алатау», поблизости пролегают маршруты троллейбусов и автобусов.\n\nНовостройка третьего класса состоит из двух зданий высотой от двенадцати до четырнадцати этажей. Здания возведены в соответствии с современными требованиями к сейсмостойкости и долговечности конструкции. Жилой комплекс «Дом на Абая» построен по монолитно-каркасной технологии. Фасады многоэтажек утеплены эффективным теплоизолятором, панорамные окна остеклены двухкамерными стеклопакетами.\n\nПодъезды домов оборудованы бесшумными комфортабельными лифтами, на нижних этажах размещены объекты собственной инфраструктуры.",
      kk: "«Абай үйі» ТҮК — мегаполис орталығындағы жайлылық пен тыныштық!\n\n«Абай үйі» тұрғын үй кешені Гагарин даңғылы мен Айманов көшесінің арасында, Абай даңғылымен қиылысуына жақын орналасқан. Жанында «Алатау» метро станциясы орналасқан, жақын жерде троллейбус және автобус маршруттары өтеді.\n\nҮшінші класты жаңа ғимарат он екіден он төрт қабатқа дейінгі биіктікті екі ғимараттан тұрады. Ғимараттар сейсмикалық төзімділік пен құрылыс ұзақтығына қатысты заманауи талаптарға сай тұрғызылған. «Абай үйі» тұрғын үй кешені монолитті-каркасты технологиямен салынған.",
      tr: "«Abay'daki Ev» — megapolis merkezinde konfor ve huzur!\n\n«Abay'daki Ev» konut kompleksi Gagarin bulvarı ile Aymanov sokağı arasında, Abay bulvarı kavşağına yakın konumdadır. Yanında «Alatau» metro istasyonu bulunmakta, yakınında troleybüs ve otobüs hatları geçmektedir.\n\nÜçüncü sınıf yeni bina on iki ile on dört kat arasında yükseklikte iki binadan oluşmaktadır. Binalar depreme dayanıklılık ve yapı dayanıklılığına ilişkin modern gereksinimlere uygun olarak inşa edilmiştir.",
      en: "«House on Abay» — comfort and tranquility in the center of the metropolis!\n\nThe residential complex «House on Abay» is located between Gagarin Avenue and Aymanov Street, near the intersection with Abay Avenue. The «Alatau» metro station is located nearby, trolleybus and bus routes run in the vicinity.\n\nThe third-class new building consists of two buildings from twelve to fourteen floors high. The buildings are constructed in accordance with modern requirements for seismic resistance and structural durability. The residential complex «House on Abay» is built using monolithic-frame technology. The facades of high-rises are insulated with effective thermal insulation, panoramic windows are glazed with double-chamber glass units.\n\nThe entrances of the buildings are equipped with quiet comfortable elevators, and infrastructure facilities are located on the lower floors.",
    },
    status: { ru: "Сдан", kk: "Тапсырылды", tr: "Tamamlandı", en: "Completed" },
    features: [
      { ru: "Центральная часть города", kk: "Қаланың орталық бөлігі", tr: "Şehrin merkezi bölümü", en: "Central part of the city" },
      { ru: "Рядом Метро «Алатау» (3 минуты пешком)", kk: "Жанында «Алатау» метросы (жаяу 3 минут)", tr: "Yakınında «Alatau» metrosu (yürüyerek 3 dakika)", en: "Near «Alatau» metro (3 minutes walk)" },
      { ru: "Зоны отдыха и аллея для прогулок", kk: "Демалыс аймақтары мен серуендеу аллеясы", tr: "Dinlenme alanları ve yürüyüş alanı", en: "Recreation areas and walking alley" },
      { ru: "На территории фитнес-клуб", kk: "Аумақта фитнес-клуб", tr: "Alanda fitness kulübü", en: "Fitness club on site" },
      { ru: "Собственный городской музей", kk: "Жеке қалалық мұражай", tr: "Kendi şehir müzesi", en: "Own city museum" },
      { ru: "Бизнес-центр в комплексе", kk: "Кешендегі бизнес-орталық", tr: "Kompleksteki iş merkezi", en: "Business center in the complex" },
    ],
  },

  // 6. LATIFA RESIDENCE
  "06-latifa-residence": {
    title: { ru: "LATIFA RESIDENCE", kk: "LATIFA RESIDENCE", tr: "LATIFA RESIDENCE", en: "LATIFA RESIDENCE" },
    subtitle: { ru: "У подножья гор", kk: "Тау етегінде", tr: "Dağların eteğinde", en: "At the foot of the mountains" },
    description: {
      ru: "ЖК Latifa Residence — это жилой комплекс Алматы, находящийся в центре города у подножия гор в Медеуском районе.\n\nЖилой комплекс «Latifa residence» спланирован в виде трех блоков из 13-этажных и 17-этажных высоток, возводимых по монолитной технологии. Вблизи горы, поэтому жилой комплекс имеет сейсмоустойчивость в 9 баллов.\n\nТерритория «Latifa residence» благоустроена для удобной жизни\n\nДетские площадки.\nЛетние беседки.\nПродуктовый магазин, аптека, центр услуг КСК.\nКруглосуточная охрана и видеонаблюдение по периметру ЖК.\nДвухуровневый подземный автопаркинг с пожаротушением, вентиляцией.",
      kk: "Latifa Residence ТҮК — Алматының Медеу ауданында, қала орталығында, тау етегінде орналасқан тұрғын үй кешені.\n\nLatifa residence тұрғын үй кешені монолитті технологиямен салынатын 13 қабатты және 17 қабатты биік ғимараттардан тұратын үш блок түрінде жоспарланған. Тауға жақын болғандықтан, тұрғын үй кешенінің сейсмикалық төзімділігі 9 балл.\n\nLatifa residence аумағы ыңғайлы өмір сүру үшін абаттандырылған\n\nБалалар алаңдары.\nЖазғы беседкалар.\nАзық-түлік дүкені, дәріхана, КСК қызметтер орталығы.\nТҮК периметрі бойынша тәулік бойы күзет және бейнебақылау.\nӨрт сөндіру және желдету жүйесі бар екі деңгейлі жер асты автотұрағы.",
      tr: "Latifa Residence — Almatı'nın Medeu bölgesinde, şehir merkezinde, dağların eteğinde yer alan bir konut kompleksidir.\n\nLatifa residence konut kompleksi, monolitik teknoloji ile inşa edilen 13 katlı ve 17 katlı yüksek binalardan oluşan üç blok şeklinde planlanmıştır. Dağa yakın olduğu için konut kompleksinin deprem dayanıklılığı 9 puandır.\n\nLatifa residence alanı rahat yaşam için düzenlenmiştir\n\nÇocuk oyun alanları.\nYaz kameriyeler.\nMarket, eczane, KSK hizmet merkezi.\nKonut kompleksi çevresinde 7/24 güvenlik ve video gözetim.\nYangın söndürme ve havalandırma sistemli iki katlı yeraltı otoparkı.",
      en: "Latifa Residence is a residential complex in Almaty, located in the city center at the foot of the mountains in the Medeu district.\n\nThe Latifa residence residential complex is planned as three blocks of 13-story and 17-story high-rises built using monolithic technology. Close to the mountains, the residential complex has a seismic resistance of 9 points.\n\nThe territory of Latifa residence is landscaped for comfortable living\n\nPlaygrounds.\nSummer gazebos.\nGrocery store, pharmacy, HOA service center.\n24-hour security and video surveillance around the perimeter of the complex.\nTwo-level underground parking with fire suppression and ventilation.",
    },
    status: { ru: "Сдан", kk: "Тапсырылды", tr: "Tamamlandı", en: "Completed" },
    features: [
      { ru: "Подножье гор", kk: "Тау етегі", tr: "Dağların eteği", en: "Foot of the mountains" },
      { ru: "Близость к реке Малая Алматинка", kk: "Кіші Алматы өзеніне жақындық", tr: "Küçük Almatinka nehrine yakınlık", en: "Proximity to the Malaya Almatinka river" },
      { ru: "Удобные подъездные пути", kk: "Ыңғайлы кіру жолдары", tr: "Rahat giriş yolları", en: "Convenient access roads" },
      { ru: "Вид на Кок-Тобе", kk: "Көктөбеге көрініс", tr: "Kök-Töbe manzarası", en: "View of Kok-Tobe" },
      { ru: "Круглосуточная охрана и аптека", kk: "Тәулік бойы күзет және дәріхана", tr: "7/24 güvenlik ve eczane", en: "24-hour security and pharmacy" },
      { ru: "Двухуровневый паркинг", kk: "Екі деңгейлі паркинг", tr: "İki katlı otopark", en: "Two-level parking" },
    ],
  },

  // 7. IZUMRUD RESIDENCE
  "07-izumrud-residence": {
    title: { ru: "IZUMRUD RESIDENCE", kk: "IZUMRUD RESIDENCE", tr: "IZUMRUD RESIDENCE", en: "IZUMRUD RESIDENCE" },
    subtitle: { ru: "Бостандыкский район", kk: "Бостандық ауданы", tr: "Bostanık bölgesi", en: "Bostandyk district" },
    description: {
      ru: "Для тех, кто выбирает для себя комфортабельный и уютный дом, создан жилой комплекс Izumrud Residence. Расположенный в Бостандыкском районе Алматы, одной из наиболее развитых частей города, он привлекает жильцов близостью всех важных социально-культурных объектов, хорошими транспортными маршрутами и общей благополучностью.\n\nНа территории комплекса также расположены:\n\nторгово-развлекательный центр;\nдвухуровневый паркинг с современной системой охраны;\nдетские игровые площадки;\nспортивные площадки для жильцов разных возрастов;\nблагоустроенные рекреационные зоны.",
      kk: "Өзіне ыңғайлы және жайлы үй таңдайтындар үшін Izumrud Residence тұрғын үй кешені жасалған. Алматының ең дамыған аудандарының бірі Бостандық ауданында орналасқан, ол тұрғындарды барлық маңызды әлеуметтік-мәдени нысандарға жақындығымен, жақсы көлік маршруттарымен және жалпы әл-ауқатымен тартады.\n\nКешен аумағында сонымен қатар орналасқан:\n\nсауда-ойын-сауық орталығы;\nзаманауи күзет жүйесі бар екі деңгейлі паркинг;\nбалалар ойын алаңдары;\nәр түрлі жастағы тұрғындарға арналған спорт алаңдары;\nабаттандырылған демалыс аймақтары.",
      tr: "Kendine rahat ve konforlu bir ev seçenler için Izumrud Residence konut kompleksi oluşturulmuştur. Almatı'nın en gelişmiş bölgelerinden biri olan Bostanık bölgesinde yer alan kompleks, sakinlerini tüm önemli sosyo-kültürel tesislere yakınlığı, iyi ulaşım güzergahları ve genel refahıyla çekmektedir.\n\nKompleks alanında ayrıca bulunmaktadır:\n\nalışveriş ve eğlence merkezi;\nmodern güvenlik sistemli iki katlı otopark;\nçocuk oyun alanları;\nfarklı yaştaki sakinler için spor alanları;\ndüzenlenmiş rekreasyon alanları.",
      en: "For those who choose a comfortable and cozy home, the Izumrud Residence residential complex was created. Located in the Bostandyk district of Almaty, one of the most developed parts of the city, it attracts residents with the proximity of all important social and cultural facilities, good transport routes and general well-being.\n\nThe complex territory also includes:\n\nshopping and entertainment center;\ntwo-level parking with modern security system;\nchildren's playgrounds;\nsports grounds for residents of different ages;\nlandscaped recreation areas.",
    },
    status: { ru: "Сдан", kk: "Тапсырылды", tr: "Tamamlandı", en: "Completed" },
    features: [
      { ru: "Парк для прогулок и занятия спортом", kk: "Серуендеу және спортпен айналысуға арналған парк", tr: "Yürüyüş ve spor için park", en: "Park for walking and sports" },
      { ru: "Подземный паркинг", kk: "Жер асты паркингі", tr: "Yeraltı otoparkı", en: "Underground parking" },
      { ru: "Бесшумные лифты", kk: "Дыбыссыз лифттер", tr: "Sessiz asansörler", en: "Silent elevators" },
      { ru: "Система видеонаблюдения", kk: "Бейнебақылау жүйесі", tr: "Video gözetim sistemi", en: "Video surveillance system" },
      { ru: "Территория для прогулок", kk: "Серуендеуге арналған аумақ", tr: "Yürüyüş alanı", en: "Walking area" },
    ],
  },

  // 8. ВОСТОЧНЫЙ ПАРК
  "08-vostochny-park": {
    title: { ru: "ВОСТОЧНЫЙ ПАРК", kk: "ВОСТОЧНЫЙ ПАРК", tr: "ВОСТОЧНЫЙ ПАРК", en: "ВОСТОЧНЫЙ ПАРК" },
    subtitle: { ru: "100м от Центрального парка", kk: "Орталық парктан 100м", tr: "Merkez parktan 100m", en: "100m from Central Park" },
    description: {
      ru: "Жилой комплекс «Восточный Парк» расположен в ста метрах от Центрального парка Культуры и Отдыха. Экологическая обстановка в районе соответствует действующим нормам.\n\nРядом с комплексом есть детский сад, школы, колледж и медицинский центр. Неподалеку филармония, множество кафе и ресторанов, поблизости имеется почта, торговые центры, супермаркеты и рынок. Ближайшая станция метро «Жибек Жолы» в полутора километрах.\n\nЖилой комплекс «Восточный Парк» представляет собой спроектированные в едином архитектурном стиле четыре дома бизнес-класса. Их высота составляет от девяти до одиннадцати этажей, в ходе строительства применяется монолитно-каркасная технология, обеспечивающая сейсмостойкость до девяти баллов.",
      kk: "«Шығыс Парк» тұрғын үй кешені Мәдениет және демалыс орталық паркінен жүз метр қашықтықта орналасқан. Аудандағы экологиялық жағдай қолданыстағы нормаларға сай.\n\nКешеннің жанында балабақша, мектептер, колледж және медициналық орталық бар. Жақын жерде филармония, көптеген кафе мен мейрамханалар, жақын маңда пошта, сауда орталықтары, супермаркеттер және базар бар. Ең жақын «Жібек Жолы» метро станциясы бір жарым километрде.\n\n«Шығыс Парк» тұрғын үй кешені бірыңғай сәулет стилінде жобаланған төрт бизнес-класс үйден тұрады. Олардың биіктігі тоғыздан он бір қабатқа дейін, құрылыс барысында тоғыз балға дейін сейсмикалық төзімділікті қамтамасыз ететін монолитті-каркасты технология қолданылады.",
      tr: "«Doğu Park» konut kompleksi, Kültür ve Dinlenme Merkez Parkı'ndan yüz metre uzaklıkta yer almaktadır. Bölgedeki ekolojik durum mevcut normlara uygundur.\n\nKompleksin yanında anaokulu, okullar, kolej ve tıp merkezi bulunmaktadır. Yakınlarda filarmoni, çok sayıda kafe ve restoran, yakın çevrede postane, alışveriş merkezleri, süpermarketler ve pazar bulunmaktadır. En yakın metro istasyonu «Jibek Joly» bir buçuk kilometre uzaklıktadır.\n\n«Doğu Park» konut kompleksi, tek bir mimari tarzda tasarlanmış dört business-class binadan oluşmaktadır. Yükseklikleri dokuz ile on bir kat arasında değişmekte olup, inşaat sırasında dokuz puana kadar deprem dayanıklılığı sağlayan monolitik-çerçeve teknolojisi uygulanmaktadır.",
      en: "The «Eastern Park» residential complex is located one hundred meters from the Central Park of Culture and Recreation. The environmental situation in the area meets current standards.\n\nNear the complex there is a kindergarten, schools, college and medical center. Nearby there is a philharmonic hall, many cafes and restaurants, there is a post office, shopping centers, supermarkets and a market nearby. The nearest metro station «Zhibek Zholy» is one and a half kilometers away.\n\nThe «Eastern Park» residential complex consists of four business-class buildings designed in a single architectural style. Their height ranges from nine to eleven floors, during construction monolithic-frame technology is used, providing seismic resistance up to nine points.",
    },
    status: { ru: "Сдан", kk: "Тапсырылды", tr: "Tamamlandı", en: "Completed" },
    features: [
      { ru: "Парки для прогулок", kk: "Серуендеуге арналған парктер", tr: "Yürüyüş parkları", en: "Parks for walking" },
      { ru: "Вид на горы", kk: "Тауларға көрініс", tr: "Dağ manzarası", en: "Mountain view" },
      { ru: "Панорамное остекление", kk: "Панорамалық әйнектеу", tr: "Panoramik cam", en: "Panoramic glazing" },
      { ru: "Удобное местоположение", kk: "Ыңғайлы орналасу", tr: "Uygun konum", en: "Convenient location" },
      { ru: "Бесшумные лифты", kk: "Дыбыссыз лифттер", tr: "Sessiz asansörler", en: "Silent elevators" },
    ],
  },

  // 9. RAMS GARDEN ALMATY
  "09-rams-garden-almaty": {
    title: { ru: "RAMS GARDEN", kk: "RAMS GARDEN", tr: "RAMS GARDEN", en: "RAMS GARDEN" },
    subtitle: { ru: "2.75 га, 13 блоков", kk: "2.75 га, 13 блок", tr: "2.75 ha, 13 blok", en: "2.75 ha, 13 blocks" },
    description: {
      ru: "Новый современный жилой комплекс бизнес-класса RAMS Garden расположился в уникальной локации на обширной территории площадью 2,75 га и состоит из 13 жилых блоков на 643 квартиры. Современный стиль архитектуры RAMS Garden оригинален и лаконичен. Каждая деталь органически вписывается в природный ландшафт и впечатляет своей изысканностью. Архитектурный ансамбль из 13 жилых блоков прекрасно дополнен зелеными зонами вокруг и внутри жилого комплекса.",
      kk: "RAMS Garden жаңа заманауи бизнес-класс тұрғын үй кешені 2,75 га ауқымды аумақта бірегей локацияда орналасқан және 643 пәтерге арналған 13 тұрғын блоктан тұрады. RAMS Garden сәулет стилі өзіндік және қарапайым. Әрбір бөлшек табиғи ландшафтқа органикалық түрде сәйкес келеді және өзінің талғампаздығымен таң қалдырады. 13 тұрғын блоктан тұратын сәулет ансамблі тұрғын үй кешенінің айналасындағы және ішіндегі жасыл аймақтармен тамаша толықтырылған.",
      tr: "RAMS Garden yeni modern business-class konut kompleksi, 2,75 hektarlık geniş bir alanda benzersiz bir konumda yer almakta ve 643 dairelik 13 konut bloğundan oluşmaktadır. RAMS Garden'ın modern mimari tarzı özgün ve yalındır. Her detay doğal peyzaja organik olarak uyum sağlar ve zarafetiyle etkilemektedir. 13 konut bloğundan oluşan mimari topluluk, konut kompleksinin etrafındaki ve içindeki yeşil alanlarla mükemmel bir şekilde tamamlanmıştır.",
      en: "The new modern business-class residential complex RAMS Garden is located in a unique location on an extensive area of 2.75 hectares and consists of 13 residential blocks for 643 apartments. The modern architectural style of RAMS Garden is original and concise. Every detail organically fits into the natural landscape and impresses with its sophistication. The architectural ensemble of 13 residential blocks is perfectly complemented by green areas around and inside the residential complex.",
    },
    status: { ru: "Сдана 1 очередь", kk: "1-кезең тапсырылды", tr: "1. Etap Tamamlandı", en: "Phase 1 Completed" },
    features: [
      { ru: "Квартиры в чистовой отделке в новом формате", kk: "Жаңа форматтағы таза өңдеудегі пәтерлер", tr: "Yeni formatta bitmiş daireler", en: "Apartments with finishing in new format" },
      { ru: "Эко-паркинг", kk: "Эко-паркинг", tr: "Eko-otopark", en: "Eco-parking" },
      { ru: "Зимний сад", kk: "Қысқы бақша", tr: "Kış bahçesi", en: "Winter garden" },
      { ru: "Глэмпинг на крыше дома", kk: "Үй шатырындағы глэмпинг", tr: "Çatıda glamping", en: "Glamping on the roof" },
      { ru: "Hobby Garden - собственный сад", kk: "Hobby Garden - жеке бақша", tr: "Hobby Garden - özel bahçe", en: "Hobby Garden - own garden" },
      { ru: "Бесключевой доступ в подъезд и квартиру", kk: "Кіреберіс пен пәтерге кілтсіз кіру", tr: "Girişe ve daireye anahtarsız erişim", en: "Keyless access to entrance and apartment" },
      { ru: "Безопасный двор и открытие шлагбаума", kk: "Қауіпсіз аула және шлагбаум ашу", tr: "Güvenli avlu ve bariyer açma", en: "Safe courtyard and barrier opening" },
      { ru: "Система безопасности с бесключевым доступом", kk: "Кілтсіз кіру жүйесі бар қауіпсіздік жүйесі", tr: "Anahtarsız erişimli güvenlik sistemi", en: "Security system with keyless access" },
      { ru: "Смарт-камеры видеонаблюдения", kk: "Смарт-бейнебақылау камералары", tr: "Akıllı video gözetim kameraları", en: "Smart video surveillance cameras" },
    ],
  },

  // 10. GRANDE VIE
  "10-grande-vie": {
    title: { ru: "GRANDE VIE", kk: "GRANDE VIE", tr: "GRANDE VIE", en: "GRANDE VIE" },
    subtitle: { ru: "Клубная резиденция в Ерменсае", kk: "Ерменсайдағы клубтық резиденция", tr: "Ermensay'da kulüp rezidansı", en: "Club residence in Ermensay" },
    description: {
      ru: "Grande Vie – клубная резиденция, которая находится в живописной горной местности и одновременно близко к центру Алматы, в микрорайоне Ерменсай по ул. Арайлы, 20/1. Он максимально отвечает принципам экологичной организации жизни. Новостройка протянулась вдоль Ремизовского ущелья и сооружается на склонах долины в полном соответствии с розой ветров.\n\nВ микрорайоне представлены все преимущества жизни в прекрасной локации рядом с центром и в окружении гор. Здесь можно отдохнуть душой от суеты и шума большого города, наслаждаясь чистым горным воздухом и тишиной. При этом менее чем в трех километрах проходит проспект Аль-Фараби, который обеспечивает удобную транспортную развязку и позволит быстро добраться в нужный район Алматы.",
      kk: "Grande Vie – көрікті таулы жерде және сонымен бірге Алматы орталығына жақын, Ерменсай шағын ауданында, Арайлы к., 20/1 мекенжайында орналасқан клубтық резиденция. Ол экологиялық өмір ұйымдастыру принциптеріне барынша сәйкес келеді. Жаңа ғимарат Ремизов шатқалы бойымен созылып, жел бағытына толық сәйкес аңғар беткейлерінде салынуда.\n\nШағын ауданда орталыққа жақын және таулармен қоршалған керемет локацияда өмір сүрудің барлық артықшылықтары ұсынылған. Мұнда үлкен қаланың шуы мен метушілігінен демалып, таза тау ауасы мен тыныштықтан ләззат алуға болады. Бұл ретте үш километрден аз жерде Әл-Фараби даңғылы өтеді, ол ыңғайлы көлік торабын қамтамасыз етеді және Алматының қажетті ауданына тез жетуге мүмкіндік береді.",
      tr: "Grande Vie – pitoresk dağlık bölgede ve aynı zamanda Almatı merkezine yakın, Ermensay mikro bölgesinde, Arayli sok., 20/1 adresinde yer alan bir kulüp rezidansıdır. Çevre dostu yaşam organizasyonu ilkelerine maksimum düzeyde uygundur. Yeni bina Remizov vadisi boyunca uzanmakta ve rüzgar yönüne tam uyumlu olarak vadi yamaçlarında inşa edilmektedir.\n\nMikro bölgede, merkeze yakın ve dağlarla çevrili harika bir konumda yaşamanın tüm avantajları sunulmaktadır. Burada büyük şehrin gürültüsünden ve koşuşturmacasından uzaklaşarak, temiz dağ havası ve sessizliğin tadını çıkarabilirsiniz. Aynı zamanda üç kilometreden az bir mesafede Al-Farabi bulvarı geçmekte, bu da rahat ulaşım bağlantısı sağlamakta ve Almatı'nın istenen bölgesine hızlıca ulaşmayı mümkün kılmaktadır.",
      en: "Grande Vie is a club residence located in a picturesque mountainous area and at the same time close to the center of Almaty, in the Ermensay microdistrict at 20/1 Araily St. It maximally meets the principles of ecological life organization. The new building stretches along the Remizov gorge and is being built on the slopes of the valley in full accordance with the wind rose.\n\nThe microdistrict offers all the advantages of living in a beautiful location near the center and surrounded by mountains. Here you can relax your soul from the hustle and bustle of the big city, enjoying clean mountain air and tranquility. At the same time, less than three kilometers away is Al-Farabi Avenue, which provides convenient transport interchange and allows you to quickly reach the desired area of Almaty.",
    },
    status: { ru: "Сдан", kk: "Тапсырылды", tr: "Tamamlandı", en: "Completed" },
    features: [
      { ru: "Прекрасная локация рядом с центром города, в окружении гор", kk: "Қала орталығына жақын, таулармен қоршалған керемет орналасу", tr: "Şehir merkezine yakın, dağlarla çevrili harika konum", en: "Beautiful location near the city center, surrounded by mountains" },
      { ru: "Финансовый центр и места притяжения алматинцев – ТРЦ Esentai Mall и VILLA Boutiques & Restaurants – в пяти минутах от жилого комплекса", kk: "Қаржы орталығы және алматылықтардың тартылу орындары – Esentai Mall СОО және VILLA Boutiques & Restaurants – тұрғын үй кешенінен бес минутта", tr: "Finans merkezi ve Almatılıların cazibe merkezleri – Esentai Mall AVM ve VILLA Boutiques & Restaurants – konut kompleksinden beş dakika uzaklıkta", en: "Financial center and attractions for Almaty residents – Esentai Mall and VILLA Boutiques & Restaurants – five minutes from the residential complex" },
      { ru: "Элитные школы рядом", kk: "Жақын жердегі элиталық мектептер", tr: "Yakınlarda elit okullar", en: "Elite schools nearby" },
      { ru: "Чистый горный воздух и собственный парк", kk: "Таза тау ауасы және жеке парк", tr: "Temiz dağ havası ve özel park", en: "Clean mountain air and own park" },
      { ru: "Комфорт и уют стильных домов с террасами и панорамными окнами", kk: "Террасалары мен панорамалық терезелері бар стильді үйлердің жайлылығы мен ыңғайлылығы", tr: "Teraslı ve panoramik pencereli şık evlerin konforu ve rahatlığı", en: "Comfort and coziness of stylish homes with terraces and panoramic windows" },
    ],
  },

  // 11. RAMS SIGNATURE
  "11-rams-signature": {
    title: { ru: "RAMS SIGNATURE", kk: "RAMS SIGNATURE", tr: "RAMS SIGNATURE", en: "RAMS SIGNATURE" },
    subtitle: { ru: "Возле Forum ТРЦ", kk: "Forum СОО жанында", tr: "Forum AVM yakınında", en: "Near Forum Mall" },
    description: {
      ru: "Rams Signature - новый жилой комплекс бизнес-класса, расположен в сердце делового центра Алматы, на пересечении улиц Байтурсынова и Тимирязева, возле ТРЦ Forum.\n\nНовостройка высотой от 8 до 9 этажей сооружается с применением передовых технологий из экологически безопасных материалов. Формы ЖК спроектированы в современном стиле с намеренным искажением типичной квадратной фигуры зданий - здесь нет углов и ограничений. У домов надежный монолитный железобетонный каркас и фундамент. Фасад новостройки отделывается фиброцементными панелями, HPL-панелями, а для облицовки террасы используется дейкинг.",
      kk: "Rams Signature - Алматының іскерлік орталығының жүрегінде, Байтұрсынов пен Тимирязев көшелерінің қиылысында, Forum СОО жанында орналасқан жаңа бизнес-класс тұрғын үй кешені.\n\n8-ден 9 қабатқа дейінгі биіктіктегі жаңа ғимарат экологиялық қауіпсіз материалдардан озық технологияларды қолдана отырып салынуда. ТҮК формалары ғимараттардың типтік шаршы пішінін әдейі бұрмалай отырып, заманауи стильде жобаланған - мұнда бұрыштар мен шектеулер жоқ. Үйлерде сенімді монолитті темірбетон қаңқасы мен іргетасы бар.",
      tr: "Rams Signature - Almatı'nın iş merkezinin kalbinde, Baytursynov ve Timiryazev sokaklarının kesişiminde, Forum AVM yakınında yer alan yeni bir business-class konut kompleksidir.\n\n8 ile 9 kat arasında yüksekliğe sahip yeni bina, çevre dostu malzemelerden ileri teknolojiler kullanılarak inşa edilmektedir. Kompleksin formları, binaların tipik kare şeklini kasıtlı olarak bozan modern tarzda tasarlanmıştır - burada köşeler ve sınırlamalar yoktur. Evlerin güvenilir monolitik betonarme çerçevesi ve temeli vardır.",
      en: "Rams Signature is a new business-class residential complex located in the heart of Almaty's business center, at the intersection of Baytursynov and Timiryazev streets, near Forum Mall.\n\nThe new building with a height of 8 to 9 floors is being constructed using advanced technologies from environmentally safe materials. The forms of the complex are designed in a modern style with intentional distortion of the typical square shape of buildings - there are no corners and restrictions here. The houses have a reliable monolithic reinforced concrete frame and foundation. The facade of the new building is finished with fiber cement panels, HPL panels, and decking is used for terrace cladding.",
    },
    status: { ru: "Строится", kk: "Салынуда", tr: "İnşaat Halinde", en: "Under Construction" },
    features: [
      { ru: "Чистовая отделка", kk: "Таза өңдеу", tr: "Bitmiş dekorasyon", en: "Finished decoration" },
      { ru: "Видеонаблюдение 24/7", kk: "24/7 бейнебақылау", tr: "7/24 video gözetim", en: "24/7 video surveillance" },
      { ru: "Work-out зоны", kk: "Work-out аймақтары", tr: "Spor alanları", en: "Workout zones" },
      { ru: "Детские площадки", kk: "Балалар алаңдары", tr: "Çocuk oyun alanları", en: "Children's playgrounds" },
      { ru: "Террасы на крыше", kk: "Шатырдағы террасалар", tr: "Çatı terasları", en: "Roof terraces" },
      { ru: "Бесключевой доступ в подъезд и квартиру", kk: "Кіреберіс пен пәтерге кілтсіз кіру", tr: "Girişe ve daireye anahtarsız erişim", en: "Keyless access to entrance and apartment" },
      { ru: "Инновационная система All-in-one", kk: "All-in-one инновациялық жүйесі", tr: "Yenilikçi All-in-one sistemi", en: "Innovative All-in-one system" },
      { ru: "Крытый паркинг", kk: "Жабық паркинг", tr: "Kapalı otopark", en: "Covered parking" },
      { ru: "Высота потолков – 3м", kk: "Төбе биіктігі – 3м", tr: "Tavan yüksekliği – 3m", en: "Ceiling height – 3m" },
      { ru: "Комнаты All-in-One", kk: "All-in-One бөлмелері", tr: "All-in-One odaları", en: "All-in-One rooms" },
    ],
  },

  // 12. RAMS SAIAHAT
  "12-rams-saiahat": {
    title: { ru: "RAMS SAIAHAT", kk: "RAMS SAIAHAT", tr: "RAMS SAIAHAT", en: "RAMS SAIAHAT" },
    subtitle: { ru: "Исторический центр", kk: "Тарихи орталық", tr: "Tarihi merkez", en: "Historic center" },
    description: {
      ru: "RAMS Saiahat – новый жилой комплекс в историческом центре Алматы на пересечении Райымбека-Суюнбая. Вместе с комплексом здесь появятся и открытый парк, башня с часами, школа. Проект RAMS Saiahat призван создать комфортные и безопасные условия для жителей комплекса и внести вклад в обновление и озеленение окружающей среды Саяхата.",
      kk: "RAMS Saiahat – Алматының тарихи орталығында Райымбек-Сүйінбай қиылысында орналасқан жаңа тұрғын үй кешені. Кешенмен бірге мұнда ашық парк, сағат мұнарасы, мектеп пайда болады. RAMS Saiahat жобасы кешен тұрғындары үшін жайлы және қауіпсіз жағдай жасауға және Саяхат қоршаған ортасын жаңартуға және көгалдандыруға үлес қосуға арналған.",
      tr: "RAMS Saiahat – Almatı'nın tarihi merkezinde Rayımbek-Suyunbay kavşağında yer alan yeni bir konut kompleksidir. Kompleksle birlikte burada açık bir park, saat kulesi ve okul da yapılacaktır. RAMS Saiahat projesi, kompleks sakinleri için konforlu ve güvenli koşullar yaratmak ve Sayahat çevresinin yenilenmesine ve yeşillendirilmesine katkıda bulunmak için tasarlanmıştır.",
      en: "RAMS Saiahat is a new residential complex in the historic center of Almaty at the intersection of Raiymbek-Suyunbay. Along with the complex, an open park, a clock tower, and a school will appear here. The RAMS Saiahat project is designed to create comfortable and safe conditions for the residents of the complex and contribute to the renewal and greening of the Sayakhat environment.",
    },
    status: { ru: "Строится", kk: "Салынуда", tr: "İnşaat Halinde", en: "Under Construction" },
    features: [
      { ru: "Открытый парк 1,3 га", kk: "1,3 га ашық парк", tr: "1,3 ha açık park", en: "Open park 1.3 ha" },
      { ru: "Школа и детсад во дворе", kk: "Ауладағы мектеп пен балабақша", tr: "Avluda okul ve anaokulu", en: "School and kindergarten in the courtyard" },
      { ru: "Бульвар с бутиками Shopping Avenue", kk: "Shopping Avenue бутиктері бар бульвар", tr: "Shopping Avenue butikli bulvar", en: "Boulevard with Shopping Avenue boutiques" },
      { ru: "Место для встреч: башня с часами", kk: "Кездесу орны: сағат мұнарасы", tr: "Buluşma yeri: saat kulesi", en: "Meeting place: clock tower" },
      { ru: "Закрытый двор без машин", kk: "Көліксіз жабық аула", tr: "Arabasız kapalı avlu", en: "Closed courtyard without cars" },
      { ru: "Высокие потолки 3 м", kk: "3 м биік төбелер", tr: "3 m yüksek tavanlar", en: "High ceilings 3 m" },
      { ru: "All-in-One: коворкинг, фитнес-зал, кинотеатр + PS, детская комната", kk: "All-in-One: коворкинг, фитнес-зал, кинотеатр + PS, балалар бөлмесі", tr: "All-in-One: coworking, spor salonu, sinema + PS, çocuk odası", en: "All-in-One: coworking, gym, cinema + PS, children's room" },
      { ru: "Рядом: Центральная мечеть, Зеленый базар, Парк 28 панфиловцев", kk: "Жақын: Орталық мешіт, Жасыл базар, 28 панфиловшылар паркі", tr: "Yakında: Merkez Camii, Yeşil Pazar, 28 Panfilov Parkı", en: "Nearby: Central Mosque, Green Bazaar, Park of 28 Panfilov Guardsmen" },
    ],
  },

  // 13. RAMS GARDEN ATYRAU
  "13-rams-garden-atyrau": {
    title: { ru: "RAMS GARDEN ATYRAU", kk: "RAMS GARDEN ATYRAU", tr: "RAMS GARDEN ATYRAU", en: "RAMS GARDEN ATYRAU" },
    subtitle: { ru: "Центр Атырау", kk: "Атырау орталығы", tr: "Atırau merkezi", en: "Atyrau center" },
    description: {
      ru: "RAMS Garden Atyrau - новый жилой комплекс бизнес-класса в самом центре Атырау! Создан для гармоничной жизни в центре городских событий. Проект расположен в центральном районе Атырау с развитой инфраструктурой для всей семьи. Комплекс сочетает в себе стильный дизайн, современные технологии и живописные прогулочные зоны. Наслаждайтесь каждым мгновением жизни!",
      kk: "RAMS Garden Atyrau - Атырау орталығындағы жаңа бизнес-класс тұрғын үй кешені! Қалалық оқиғалар орталығында үйлесімді өмір сүру үшін жасалған. Жоба Атыраудың бүкіл отбасына арналған дамыған инфрақұрылымы бар орталық ауданында орналасқан. Кешен стильді дизайнды, заманауи технологияларды және көрікті серуендеу аймақтарын біріктіреді. Өмірдің әр сәтінен ләззат алыңыз!",
      tr: "RAMS Garden Atyrau - Atırau'nun tam merkezinde yeni bir business-class konut kompleksi! Şehir olaylarının merkezinde uyumlu bir yaşam için yaratılmıştır. Proje, tüm aile için gelişmiş altyapıya sahip Atırau'nun merkezi bölgesinde yer almaktadır. Kompleks, şık tasarımı, modern teknolojileri ve pitoresk yürüyüş alanlarını bir arada sunmaktadır. Hayatın her anının tadını çıkarın!",
      en: "RAMS Garden Atyrau is a new business-class residential complex in the very center of Atyrau! Created for harmonious life in the center of city events. The project is located in the central district of Atyrau with developed infrastructure for the whole family. The complex combines stylish design, modern technologies and picturesque walking areas. Enjoy every moment of life!",
    },
    status: { ru: "Строится", kk: "Салынуда", tr: "İnşaat Halinde", en: "Under Construction" },
    features: [
      { ru: "Безопасный, закрытый двор", kk: "Қауіпсіз, жабық аула", tr: "Güvenli, kapalı avlu", en: "Safe, closed courtyard" },
      { ru: "Эко-паркинг", kk: "Эко-паркинг", tr: "Eko-otopark", en: "Eco-parking" },
      { ru: "Детские эко-площадки, workout-зоны", kk: "Балаларға арналған эко-алаңдар, workout-аймақтар", tr: "Çocuk eko-alanları, spor alanları", en: "Children's eco-playgrounds, workout zones" },
      { ru: "Передовые технологии: видеодомофон, смарт-замок", kk: "Озық технологиялар: бейнедомофон, смарт-құлып", tr: "İleri teknolojiler: video interkom, akıllı kilit", en: "Advanced technologies: video intercom, smart lock" },
      { ru: "Насыщенное озеленение внутренней территории", kk: "Ішкі аумақтың қарқынды көгалдандырылуы", tr: "İç alanın yoğun yeşillendirilmesi", en: "Intensive greening of the internal territory" },
    ],
  },

  // 14. RAMS CENTRE (ORTAU MARRIOTT BC)
  "14-ortau-marriott-bc": {
    title: { ru: "RAMS CENTRE", kk: "RAMS CENTRE", tr: "RAMS CENTRE", en: "RAMS CENTRE" },
    subtitle: { ru: "БЦ класса А с Marriott", kk: "Marriott бар А класты БО", tr: "Marriott'lu A sınıfı İM", en: "Class A BC with Marriott" },
    description: {
      ru: "Здание находится по адресу: пр. Сакена Сейфуллина. Локация бизнес-центра очень комфортная, от него достаточно оперативно можно добраться до любой части города. Кроме того, район хорошо развит с точки зрения деловой и торговой инфраструктуры, поблизости имеется все необходимое для удобства сотрудников вашей компании.\n\nОфисное помещение идеально подойдет для вашей компании. Бизнес центр соответствует всем критериям класса A. Офисный комплекс имеет современные инженерные коммуникации. Офисы сдаются с отделкой/без отделки. В общих зонах RAMS Centre выполнен отличный ремонт. Деловой комплекс охраняется, состав резидентов офисного комплекса удачный.\n\nБЦ RAMS Centre — современный бизнес-центр класса «А» в самом сердце Алматы, на пересечении улиц Сейфуллина и Сатпаева. Это престижный адрес, подчёркивающий статус и надёжность компании.",
      kk: "Ғимарат мекенжайы: Сәкен Сейфуллин даңғылы. Бизнес-орталықтың орналасуы өте ыңғайлы, одан қаланың кез келген бөлігіне тез жетуге болады. Сонымен қатар, аудан іскерлік және сауда инфрақұрылымы тұрғысынан жақсы дамыған, жақын маңда сіздің компания қызметкерлерінің ыңғайлылығы үшін қажеттінің бәрі бар.\n\nКеңсе үй-жайы сіздің компанияңызға өте қолайлы. Бизнес-орталық А класының барлық критерийлеріне сай келеді. Кеңсе кешенінде заманауи инженерлік коммуникациялар бар.\n\nRAMS Centre БО — Алматының жүрегінде, Сейфуллин мен Сатпаев көшелерінің қиылысында орналасқан заманауи «А» класты бизнес-орталық. Бұл компанияның мәртебесі мен сенімділігін көрсететін беделді мекенжай.",
      tr: "Bina, Saken Seifullin Bulvarı adresinde yer almaktadır. İş merkezinin konumu çok rahat, buradan şehrin herhangi bir yerine hızlıca ulaşılabilir. Ayrıca bölge, iş ve ticaret altyapısı açısından iyi gelişmiş olup, yakınlarda şirket çalışanlarınızın rahatlığı için gerekli her şey mevcuttur.\n\nOfis alanı şirketiniz için ideal olacaktır. İş merkezi A sınıfının tüm kriterlerini karşılamaktadır. Ofis kompleksi modern mühendislik iletişimlerine sahiptir.\n\nRAMS Centre — Almatı'nın tam kalbinde, Seifullin ve Satpayev sokaklarının kesişiminde yer alan modern «A» sınıfı bir iş merkezidir. Bu, şirketin statüsünü ve güvenilirliğini vurgulayan prestijli bir adrestir.",
      en: "The building is located at Saken Seifullin Avenue. The location of the business center is very comfortable, from it you can quickly reach any part of the city. In addition, the area is well developed in terms of business and retail infrastructure, everything necessary for the convenience of your company's employees is available nearby.\n\nThe office space is ideal for your company. The business center meets all Class A criteria. The office complex has modern engineering communications. Offices are rented with/without finishing. Common areas of RAMS Centre have excellent renovation. The business complex is guarded, the composition of office complex residents is successful.\n\nRAMS Centre is a modern Class A business center in the very heart of Almaty, at the intersection of Seifullin and Satpayev streets. This is a prestigious address that emphasizes the status and reliability of the company.",
    },
    status: { ru: "Завершен", kk: "Аяқталды", tr: "Tamamlandı", en: "Completed" },
    features: [
      { ru: "A-Class офисные пространства для аренды с прямым доступом к сервисам отеля Marriott", kk: "Marriott қонақ үйінің қызметтеріне тікелей қол жетімділігі бар А класты жалға берілетін кеңсе кеңістіктері", tr: "Marriott otel hizmetlerine doğrudan erişimli A-Class kiralık ofis alanları", en: "A-Class office spaces for rent with direct access to Marriott hotel services" },
      { ru: "Премиальные торговые площади для мировых fashion-брендов, ювелирных бутиков, кафе и ресторанов", kk: "Әлемдік сәнді брендтер, зергерлік бутиктер, кафе мен мейрамханаларға арналған премиум сауда алаңдары", tr: "Dünya moda markaları, mücevher butikleri, kafeler ve restoranlar için premium ticari alanlar", en: "Premium retail spaces for world fashion brands, jewelry boutiques, cafes and restaurants" },
      { ru: "Первый в Алматы пятизвёздочный отель международного бренда Marriott", kk: "Алматыдағы алғашқы Marriott халықаралық брендінің бес жұлдызды қонақ үйі", tr: "Almatı'daki ilk Marriott uluslararası markasının beş yıldızlı oteli", en: "The first five-star Marriott international brand hotel in Almaty" },
      { ru: "Панорамные окна, просторные open-space зоны и потолки высотой 4,4 м", kk: "Панорамалық терезелер, кең open-space аймақтары және 4,4 м биіктіктегі төбелер", tr: "Panoramik pencereler, geniş açık alan bölgeleri ve 4,4 m yüksekliğinde tavanlar", en: "Panoramic windows, spacious open-space zones and 4.4 m high ceilings" },
      { ru: "Круглосуточная охрана и вместительный подземный паркинг", kk: "Тәулік бойы күзет және сыйымды жер асты паркингі", tr: "7/24 güvenlik ve geniş yeraltı otoparkı", en: "24-hour security and spacious underground parking" },
    ],
  },

  // 15. RAMS EVO
  "15-rams-evo": {
    title: { ru: "RAMS EVO", kk: "RAMS EVO", tr: "RAMS EVO", en: "RAMS EVO" },
    subtitle: { ru: "Ауэзова-Гоголя", kk: "Әуезов-Гоголь", tr: "Auezov-Gogol", en: "Auezov-Gogol" },
    description: {
      ru: "Жилой комплекс Rams Evo расположен в динамичном центральном районе Алматы на пересечении улиц Ауэзова и Гоголя, недалеко от Парка 28 панфиловцев.\n\nВ строительстве проекта будет реализован подход EVO Concept с пошаговым благоустройством всего жилого пространства: начиная от квартала, переходя ко двору, дому и квартире.\n\nRAMS EVO задаст новые стандарты качества жизни. Здесь будет собрана экосистема из мест для работы, спорта, игр, творчества и семейного отдыха. В шаге от дома будет сформирована насыщенная среда для вашего досуга.",
      kk: "Rams Evo тұрғын үй кешені Алматының серпінді орталық ауданында Әуезов пен Гоголь көшелерінің қиылысында, 28 панфиловшылар паркіне жақын жерде орналасқан.\n\nЖобаны салу кезінде EVO Concept тәсілі жүзеге асырылады, ол бүкіл тұрғын кеңістікті кезең-кезеңімен абаттандыруды қамтиды: кварталдан бастап, ауланы, үйді және пәтерді қамтиды.\n\nRAMS EVO өмір сапасының жаңа стандарттарын белгілейді. Мұнда жұмыс, спорт, ойындар, шығармашылық және отбасылық демалыс орындарының экожүйесі жиналады. Үйден бір қадамда сіздің бос уақытыңызға арналған қанық орта қалыптасады.",
      tr: "Rams Evo konut kompleksi, Almatı'nın dinamik merkezi bölgesinde Auezov ve Gogol sokaklarının kesişiminde, 28 Panfilov Parkı yakınında yer almaktadır.\n\nProjenin inşaatında, tüm yaşam alanının adım adım iyileştirilmesini içeren EVO Concept yaklaşımı uygulanacaktır: mahalleden başlayarak avluya, binaya ve daireye geçiş.\n\nRAMS EVO yeni yaşam kalitesi standartları belirleyecektir. Burada iş, spor, oyunlar, yaratıcılık ve aile eğlencesi için mekanlardan oluşan bir ekosistem toplanacaktır. Evden bir adım uzaklıkta boş zamanlarınız için zengin bir ortam oluşturulacaktır.",
      en: "The Rams Evo residential complex is located in the dynamic central district of Almaty at the intersection of Auezov and Gogol streets, near the Park of 28 Panfilov Guardsmen.\n\nThe EVO Concept approach will be implemented in the construction of the project with step-by-step improvement of the entire living space: starting from the block, moving to the courtyard, building and apartment.\n\nRAMS EVO will set new standards of quality of life. Here an ecosystem of places for work, sports, games, creativity and family recreation will be assembled. A rich environment for your leisure will be formed a step away from home.",
    },
    status: { ru: "Строится", kk: "Салынуда", tr: "İnşaat Halinde", en: "Under Construction" },
    features: [
      { ru: "Зоны для отдыха", kk: "Демалысқа арналған аймақтар", tr: "Dinlenme alanları", en: "Recreation zones" },
      { ru: "Игровые, спортивные площадки", kk: "Ойын, спорт алаңдары", tr: "Oyun ve spor alanları", en: "Play and sports grounds" },
      { ru: "Высокие потолки", kk: "Биік төбелер", tr: "Yüksek tavanlar", en: "High ceilings" },
      { ru: "Авторский дизайн", kk: "Авторлық дизайн", tr: "Özgün tasarım", en: "Author's design" },
      { ru: "Собственная школа во дворе", kk: "Ауладағы жеке мектеп", tr: "Avluda özel okul", en: "Own school in the courtyard" },
      { ru: "ALL-IN-ONE: коворкинг, фитнес-зал, кинотеатр и детская комната", kk: "ALL-IN-ONE: коворкинг, фитнес-зал, кинотеатр және балалар бөлмесі", tr: "ALL-IN-ONE: coworking, spor salonu, sinema ve çocuk odası", en: "ALL-IN-ONE: coworking, gym, cinema and children's room" },
    ],
  },

  // 16. KERUEN CITY
  "16-rams-keruen-city": {
    title: { ru: "KERUEN CITY", kk: "KERUEN CITY", tr: "KERUEN CITY", en: "KERUEN CITY" },
    subtitle: { ru: "Астана", kk: "Астана", tr: "Astana", en: "Astana" },
    description: {
      ru: "Keruen City - жилой комплекс, где пересекаются прошлое и будущее, создавая идеальные условия для вашей жизни. Здесь каждый уголок продуман до мелочей, чтобы ваша семья могла наслаждаться комфортом, безопасностью и красотой природы, не теряя связи с динамичным ритмом города.",
      kk: "Keruen City - өткен мен болашақ қиылысатын, сіздің өміріңіз үшін тамаша жағдайлар жасайтын тұрғын үй кешені. Мұнда әрбір бұрыш ойластырылған, сіздің отбасыңыз қаланың серпінді ырғағымен байланысын жоғалтпай, жайлылықтан, қауіпсіздіктен және табиғат сұлулығынан ләззат ала алады.",
      tr: "Keruen City - geçmiş ve geleceğin kesiştiği, yaşamınız için ideal koşullar yaratan bir konut kompleksidir. Burada her köşe en ince ayrıntısına kadar düşünülmüştür, böylece aileniz şehrin dinamik ritmiyle bağlantıyı kaybetmeden konfor, güvenlik ve doğanın güzelliğinin tadını çıkarabilir.",
      en: "Keruen City is a residential complex where the past and future intersect, creating ideal conditions for your life. Here every corner is thought out to the smallest detail, so that your family can enjoy comfort, safety and the beauty of nature without losing touch with the dynamic rhythm of the city.",
    },
    status: { ru: "Строится", kk: "Салынуда", tr: "İnşaat Halinde", en: "Under Construction" },
    features: [
      { ru: "Зоны для отдыха", kk: "Демалысқа арналған аймақтар", tr: "Dinlenme alanları", en: "Recreation zones" },
      { ru: "Игровые, спортивные площадки", kk: "Ойын, спорт алаңдары", tr: "Oyun ve spor alanları", en: "Play and sports grounds" },
      { ru: "Высокие потолки", kk: "Биік төбелер", tr: "Yüksek tavanlar", en: "High ceilings" },
      { ru: "Авторский дизайн", kk: "Авторлық дизайн", tr: "Özgün tasarım", en: "Author's design" },
      { ru: "ТРЦ Aport Mall рядом", kk: "Жақында Aport Mall СОО", tr: "Yakınında Aport Mall AVM", en: "Aport Mall nearby" },
      { ru: "Рынок Алтын Орда рядом", kk: "Жақында Алтын Орда базары", tr: "Yakınında Altın Orda pazarı", en: "Altyn Orda market nearby" },
      { ru: "Новая станция метро в ближайшем будущем", kk: "Жақын болашақта жаңа метро станциясы", tr: "Yakın gelecekte yeni metro istasyonu", en: "New metro station in the near future" },
      { ru: "Современный подземный паркинг", kk: "Заманауи жер асты паркингі", tr: "Modern yeraltı otoparkı", en: "Modern underground parking" },
      { ru: "Закрытые дворы без машин", kk: "Көліксіз жабық аулалар", tr: "Arabasız kapalı avlular", en: "Closed courtyards without cars" },
      { ru: "Видеонаблюдение 24/7, видеодомофоны, смарт-замки", kk: "24/7 бейнебақылау, бейнедомофондар, смарт-құлыптар", tr: "7/24 video gözetim, video interkomlar, akıllı kilitler", en: "24/7 video surveillance, video intercoms, smart locks" },
    ],
  },

  // 17. ORTAU
  "17-ortau": {
    title: { ru: "ORTAU", kk: "ORTAU", tr: "ORTAU", en: "ORTAU" },
    subtitle: { ru: "Бостандыкский район", kk: "Бостандық ауданы", tr: "Bostanık bölgesi", en: "Bostandyk district" },
    description: {
      ru: "Инновационный ЖК Ortau, расположенный в Бостандыкском районе, прямо в центре города, совместил в себе невероятные до сих пор возможности для своих жильцов и благоприятную экологию.\n\nДля будущих жильцов доступны квартиры в 2-3-4-комнатных планировках в предчистовой отделке. В стоимость уже входит остекление стеклопакетами и витражное остекление балконов.\n\nКонцептуальный ЖК Ortau – это пять 10-этажных домов, спроектированных в современной стилистике. Строительство ведется с применением надежной и долговечной монолитно-каркасной технологии.",
      kk: "Бостандық ауданында, қала орталығында орналасқан инновациялық Ortau ТҮК өзінің тұрғындары үшін бұрын-соңды болмаған мүмкіндіктер мен қолайлы экологияны біріктірді.\n\nБолашақ тұрғындар үшін алдын ала өңдеуде 2-3-4 бөлмелі жоспарларда пәтерлер қол жетімді. Бағаға стеклопакетпен әйнектеу және балкондарды витраждық әйнектеу кіреді.\n\nТұжырымдамалық Ortau ТҮК – заманауи стильде жобаланған бес 10 қабатты үй. Құрылыс сенімді және ұзақ мерзімді монолитті-каркасты технологияны қолдана отырып жүргізіледі.",
      tr: "Bostanık bölgesinde, şehir merkezinde yer alan yenilikçi Ortau konut kompleksi, sakinleri için şimdiye kadar görülmemiş olanakları ve elverişli ekolojiyi bir araya getirmiştir.\n\nGelecekteki sakinler için ön bitirme ile 2-3-4 odalı planlarda daireler mevcuttur. Fiyata cam paketlerle camlama ve balkonların vitray camlaması dahildir.\n\nKavramsal Ortau konut kompleksi, modern tarzda tasarlanmış beş 10 katlı binadan oluşmaktadır. İnşaat, güvenilir ve dayanıklı monolitik-çerçeve teknolojisi kullanılarak yürütülmektedir.",
      en: "The innovative Ortau residential complex, located in the Bostandyk district, right in the city center, has combined incredible opportunities for its residents and favorable ecology.\n\nFor future residents, apartments in 2-3-4-room layouts with pre-finishing are available. The price already includes double-glazed windows and stained glass glazing of balconies.\n\nThe conceptual Ortau residential complex consists of five 10-story buildings designed in modern style. Construction is carried out using reliable and durable monolithic-frame technology.",
    },
    status: { ru: "Сдан", kk: "Тапсырылды", tr: "Tamamlandı", en: "Completed" },
    features: [
      { ru: "18 многофункциональных комнат внутри ЖК, доступных для аренды каждому жильцу дома", kk: "ТҮК ішінде үйдің әрбір тұрғынына жалға алуға қол жетімді 18 көп функциялы бөлме", tr: "Konut kompleksi içinde her sakine kiralanabilir 18 çok işlevli oda", en: "18 multifunctional rooms inside the complex available for rent to every resident" },
      { ru: "Развитая инфраструктура", kk: "Дамыған инфрақұрылым", tr: "Gelişmiş altyapı", en: "Developed infrastructure" },
      { ru: "Закрытый двор", kk: "Жабық аула", tr: "Kapalı avlu", en: "Closed courtyard" },
      { ru: "Принцип «двор без машин»", kk: "«Көліксіз аула» қағидаты", tr: "«Arabasız avlu» ilkesi", en: "«Courtyard without cars» principle" },
      { ru: "Большой двухуровневый паркинг", kk: "Үлкен екі деңгейлі паркинг", tr: "Büyük iki katlı otopark", en: "Large two-level parking" },
      { ru: "Детская площадка с качелями, горками, песочницами", kk: "Әткеншектері, сырғанақтары, құм жәшіктері бар балалар алаңы", tr: "Salıncaklar, kaydıraklar, kum havuzları ile çocuk oyun alanı", en: "Playground with swings, slides, sandboxes" },
    ],
  },

  // 18. LAMIYA
  "18-lamiya": {
    title: { ru: "LAMIYA", kk: "LAMIYA", tr: "LAMIYA", en: "LAMIYA" },
    subtitle: { ru: "Старый центр", kk: "Ескі орталық", tr: "Eski merkez", en: "Old center" },
    description: {
      ru: "ЖК Lamiya – это новый взгляд на старый культурный центр Алматы. Футуристичный фасад и множество коммерческих объектов, которые появятся сразу после запуска жилого комплекса добавят свежести и возможностей многим кварталам.\n\nДанная новостройка располагается между улицами Курмангазы и Шарипова с одной стороны и улицами Шевченко и Шагабутдинова с другой. Неподалеку находится живописный парк с аттракционами «Fantasy» и сквер Ахмета Байтурсынова, торговый центр Promenade и множество популярных кофеен.\n\nТринадцатиэтажный дом, состоящий из пяти секций, спроектирован с учетом современных норм и требований относительно безопасности.",
      kk: "Lamiya ТҮК – Алматының ескі мәдени орталығына жаңаша көзқарас. Футуристік қасбет және тұрғын үй кешені іске қосылғаннан кейін бірден пайда болатын көптеген коммерциялық нысандар көптеген кварталдарға жаңалық пен мүмкіндіктер қосады.\n\nБұл жаңа ғимарат бір жағынан Құрманғазы мен Шарипов көшелерінің арасында, екінші жағынан Шевченко мен Шағабутдинов көшелерінің арасында орналасқан. Жақын жерде «Fantasy» аттракциондары бар көрікті парк және Ахмет Байтұрсынов скверы, Promenade сауда орталығы және көптеген танымал кофеханалар орналасқан.\n\nБес секциядан тұратын он үш қабатты үй қауіпсіздікке қатысты заманауи нормалар мен талаптарды ескере отырып жобаланған.",
      tr: "Lamiya konut kompleksi – Almatı'nın eski kültürel merkezine yeni bir bakış açısıdır. Fütüristik cephe ve konut kompleksinin açılmasından hemen sonra ortaya çıkacak çok sayıda ticari tesis, birçok mahalleye tazelik ve fırsatlar katacaktır.\n\nBu yeni bina, bir tarafta Kurmangazy ve Sharipov sokakları, diğer tarafta Shevchenko ve Shagabutdinov sokakları arasında yer almaktadır. Yakınlarda «Fantasy» eğlence parkı ve Ahmet Baytursynov meydanı, Promenade alışveriş merkezi ve birçok popüler kahvehane bulunmaktadır.\n\nBeş bölümden oluşan on üç katlı bina, güvenlikle ilgili modern normlara ve gereksinimlere uygun olarak tasarlanmıştır.",
      en: "Lamiya residential complex is a new look at the old cultural center of Almaty. The futuristic facade and many commercial facilities that will appear immediately after the launch of the residential complex will add freshness and opportunities to many blocks.\n\nThis new building is located between Kurmangazy and Sharipov streets on one side and Shevchenko and Shagabutdinov streets on the other. Nearby is the picturesque Fantasy amusement park and Akhmet Baitursynov square, Promenade shopping center and many popular coffee shops.\n\nThe thirteen-story building consisting of five sections is designed in accordance with modern safety standards and requirements.",
    },
    status: { ru: "Сдан", kk: "Тапсырылды", tr: "Tamamlandı", en: "Completed" },
    features: [
      { ru: "Собственная система развлечений прямо внутри комплекса: салоны красоты, кафе, модные бутики и магазины", kk: "Кешен ішіндегі жеке ойын-сауық жүйесі: сұлулық салондары, кафелер, сәнді бутиктер мен дүкендер", tr: "Kompleks içindeki özel eğlence sistemi: güzellik salonları, kafeler, moda butikleri ve mağazalar", en: "Own entertainment system right inside the complex: beauty salons, cafes, fashion boutiques and shops" },
      { ru: "Расположение в тихом старом центре", kk: "Тыныш ескі орталықта орналасу", tr: "Sessiz eski merkezde konum", en: "Location in the quiet old center" },
      { ru: "Детская и спортивная площадка", kk: "Балалар және спорт алаңы", tr: "Çocuk ve spor alanı", en: "Children's and sports playground" },
      { ru: "Скверы с клумбами и деревьями", kk: "Гүлзарлары мен ағаштары бар скверлер", tr: "Çiçek tarhları ve ağaçları olan meydanlar", en: "Squares with flower beds and trees" },
    ],
  },

  // 19. LA VERDE
  "19-la-verde": {
    title: { ru: "LA VERDE", kk: "LA VERDE", tr: "LA VERDE", en: "LA VERDE" },
    subtitle: { ru: "Элит, у подножья гор", kk: "Элит, тау етегінде", tr: "Elit, dağların eteğinde", en: "Elite, at the foot of the mountains" },
    description: {
      ru: "Презентабельная новостройка элитного класса ЖК La Verde расположился в Медеуском районе города Алматы у подножья горного массива и рядом с речкой Есентай. Это живописное место с хорошей экологией, которое гармонично сочетает все прелести городской и загородной жизни. Рядом нет крупных промышленных предприятий и загруженных автомагистралей.\n\nРоскошный ЖК Ля Верде создан по индивидуальному проекту, который ориентирован на местный климат и сейсмическую ситуацию. В состав жилого комплекса входит целых 10 трехэтажных домов класса «элит» с панорамными окнами, которые позволяют жильцам в любое время любоваться красивыми пейзажами.",
      kk: "La Verde элит класты презентабельді жаңа ғимараты Алматы қаласының Медеу ауданында тау массивінің етегінде және Есентай өзенінің жанында орналасқан. Бұл жақсы экологиясы бар көрікті жер, ол қалалық және қала сыртындағы өмірдің барлық әсемдіктерін үйлесімді түрде біріктіреді. Жақын жерде ірі өнеркәсіптік кәсіпорындар мен жүктелген автомагистральдар жоқ.\n\nСәнді La Verde ТҮК жергілікті климат пен сейсмикалық жағдайға бағытталған жеке жоба бойынша жасалған. Тұрғын үй кешенінің құрамына тұрғындарға кез келген уақытта әдемі пейзаждарды тамашалауға мүмкіндік беретін панорамалық терезелері бар 10 үш қабатты «элит» класты үй кіреді.",
      tr: "La Verde elit sınıf prestijli yeni binası, Almatı'nın Medeu bölgesinde dağ silsilesinin eteğinde ve Esentay nehri yakınında yer almaktadır. Bu, şehir ve kır yaşamının tüm güzelliklerini uyumlu bir şekilde birleştiren, iyi ekolojiye sahip pitoresk bir yerdir. Yakınlarda büyük sanayi tesisleri ve yoğun otobanlar yoktur.\n\nLüks La Verde konut kompleksi, yerel iklime ve sismik duruma yönelik bireysel bir proje kapsamında oluşturulmuştur. Konut kompleksi, sakinlerin her zaman güzel manzaraların tadını çıkarmasına olanak tanıyan panoramik pencereli 10 adet üç katlı «elit» sınıf binadan oluşmaktadır.",
      en: "The prestigious elite-class new building La Verde is located in the Medeu district of Almaty at the foot of the mountain range and near the Esentay River. This is a picturesque place with good ecology that harmoniously combines all the charms of urban and suburban life. There are no large industrial enterprises and busy highways nearby.\n\nThe luxurious La Verde residential complex was created according to an individual project focused on the local climate and seismic situation. The residential complex includes 10 three-story elite-class buildings with panoramic windows that allow residents to enjoy beautiful landscapes at any time.",
    },
    status: { ru: "Сдан", kk: "Тапсырылды", tr: "Tamamlandı", en: "Completed" },
    features: [
      { ru: "Собственная уникальная для Казахстана система All-in-One", kk: "Қазақстан үшін бірегей жеке All-in-One жүйесі", tr: "Kazakistan için benzersiz özel All-in-One sistemi", en: "Own unique All-in-One system for Kazakhstan" },
      { ru: "Расположение в верхней экологичной части города", kk: "Қаланың жоғарғы экологиялық бөлігінде орналасу", tr: "Şehrin üst ekolojik bölümünde konum", en: "Location in the upper ecological part of the city" },
      { ru: "Персональный выход на реку Есентай", kk: "Есентай өзеніне жеке шығу", tr: "Esentay nehrine özel çıkış", en: "Personal access to the Esentay River" },
      { ru: "Собственный теннисный корт", kk: "Жеке теннис корты", tr: "Özel tenis kortu", en: "Own tennis court" },
      { ru: "Большой двухуровневый паркинг", kk: "Үлкен екі деңгейлі паркинг", tr: "Büyük iki katlı otopark", en: "Large two-level parking" },
      { ru: "Круглосуточное видеонаблюдение и охранная служба", kk: "Тәулік бойы бейнебақылау және күзет қызметі", tr: "7/24 video gözetim ve güvenlik hizmeti", en: "24-hour video surveillance and security service" },
    ],
  },

  // 20. ILE DE FRANCE
  "20-ile-de-france": {
    title: { ru: "ILE DE FRANCE", kk: "ILE DE FRANCE", tr: "ILE DE FRANCE", en: "ILE DE FRANCE" },
    subtitle: { ru: "Французский стиль", kk: "Француз стилі", tr: "Fransız tarzı", en: "French style" },
    description: {
      ru: "Утонченная красота, тонкий шарм, ощущение защищенности и благополучия, словом всё, чем так притягивает к себе Париж, было воплощено в новом элитном жилом комплексе Ile de France на Назарбаева–Хаджимукана.\n\nКультурно-жилищный комплекс «Ile De France» в охраняемой природной части Алматы с видом на Кок тобе. Наслаждайтесь видом Кок тобе из окна собственной квартиры. А вечером гуляйте с детьми, занимайтесь спортом или уютно отдохните вблизи символа Алматы. Оцените всю свежесть чистейшего горного воздуха, после душных городских кварталов среди смога, пыли и грязи. Подарите себе и своим детям тишину и покой после тяжёлого трудового дня без суматохи, пробок и шума городских кварталов.",
      kk: "Нәзік сұлулық, жұқа сарын, қорғаныс пен әл-ауқат сезімі, бір сөзбен айтқанда, Парижді өзіне тартатынның бәрі Назарбаев–Хаджімұқан көшесіндегі жаңа элиталық Ile de France тұрғын үй кешенінде жүзеге асырылды.\n\n«Ile De France» мәдени-тұрғын үй кешені Алматының қорғалатын табиғи бөлігінде Көктөбеге көрініспен орналасқан. Өз пәтеріңіздің терезесінен Көктөбе көрінісінен ләззат алыңыз. Ал кешке балалармен серуендеңіз, спортпен айналысыңыз немесе Алматы символының жанында жайлы демалыңыз. Смог, шаң мен кірдің ортасындағы тұншықтыратын қала кварталдарынан кейін таза тау ауасының барлық тазалығын бағалаңыз.",
      tr: "İnce güzellik, zarif çekicilik, korunma ve refah hissi, kısacası Paris'in bu kadar çekici kıldığı her şey, Nazarbayev-Hajimukan'daki yeni elit konut kompleksi Ile de France'da hayata geçirilmiştir.\n\n«Ile De France» kültürel-konut kompleksi, Kok-Tobe manzaralı Almatı'nın korunan doğal bölgesinde yer almaktadır. Kendi dairenizin penceresinden Kok-Tobe manzarasının tadını çıkarın. Akşamları çocuklarla yürüyün, spor yapın veya Almatı sembolünün yakınında rahatça dinlenin. Duman, toz ve kir arasındaki bunaltıcı şehir bloklarından sonra temiz dağ havasının tüm tazeliğini değerlendirin.",
      en: "Refined beauty, subtle charm, a sense of security and well-being, in short, everything that Paris attracts so much was embodied in the new elite residential complex Ile de France on Nazarbayev-Khadzhimukan.\n\nThe cultural and residential complex «Ile De France» is located in the protected natural part of Almaty with a view of Kok-Tobe. Enjoy the view of Kok-Tobe from your own apartment window. And in the evening, walk with children, play sports or relax comfortably near the symbol of Almaty. Appreciate all the freshness of the purest mountain air after the stuffy city blocks among smog, dust and dirt. Give yourself and your children peace and quiet after a hard day's work without the hustle and bustle, traffic jams and noise of city blocks.",
    },
    status: { ru: "Сдан", kk: "Тапсырылды", tr: "Tamamlandı", en: "Completed" },
    features: [
      { ru: "При строительстве использовались материалы премиум-класса", kk: "Құрылыста премиум-класс материалдары қолданылды", tr: "İnşaatta premium sınıf malzemeler kullanıldı", en: "Premium-class materials were used in construction" },
      { ru: "Сейсмостойкость 9 баллов", kk: "9 балл сейсмикалық төзімділік", tr: "9 puan deprem dayanıklılığı", en: "Seismic resistance 9 points" },
      { ru: "Бесшумные скоростные лифты", kk: "Дыбыссыз жылдам лифттер", tr: "Sessiz hızlı asansörler", en: "Silent high-speed elevators" },
      { ru: "Уникальный архитектурный стиль", kk: "Бірегей сәулет стилі", tr: "Benzersiz mimari tarz", en: "Unique architectural style" },
      { ru: "Современные игровые и спортивные площадки", kk: "Заманауи ойын және спорт алаңдары", tr: "Modern oyun ve spor alanları", en: "Modern playgrounds and sports grounds" },
      { ru: "Благоустроенные в французском стиле дворы", kk: "Француз стилінде абаттандырылған аулалар", tr: "Fransız tarzında düzenlenmiş avlular", en: "Courtyards landscaped in French style" },
      { ru: "Развитая инфраструктура – бутики, салон красоты, цветочные магазины, кофейня, кондитерская и ресторан", kk: "Дамыған инфрақұрылым – бутиктер, сұлулық салоны, гүл дүкендері, кофехана, кондитерлік және мейрамхана", tr: "Gelişmiş altyapı – butikler, güzellik salonu, çiçekçiler, kahvehane, pastane ve restoran", en: "Developed infrastructure – boutiques, beauty salon, flower shops, coffee shop, confectionery and restaurant" },
      { ru: "146 парковочных наземных и подземных мест", kk: "146 жер үсті және жер асты тұрақ орны", tr: "146 yerüstü ve yeraltı park yeri", en: "146 ground and underground parking spaces" },
      { ru: "В доступной близости каток «Медео» и горнолыжная база «Шымбулак»", kk: "Қол жетімді жақындықта «Медеу» мұз айдыны және «Шымбұлақ» тау шаңғысы базасы", tr: "Erişilebilir mesafede «Medeo» buz pateni pisti ve «Shymbulak» kayak merkezi", en: "Within easy reach of Medeo skating rink and Shymbulak ski resort" },
    ],
  },

  // 21. FORUM RESIDENCE
  "21-forum-residence": {
    title: { ru: "FORUM RESIDENCE", kk: "FORUM RESIDENCE", tr: "FORUM RESIDENCE", en: "FORUM RESIDENCE" },
    subtitle: { ru: "316-й квартал Алматы", kk: "Алматының 316-шы кварталы", tr: "Almatı'nın 316. mahallesi", en: "316th quarter of Almaty" },
    description: {
      ru: "ЖК Forum Residence – это привлекательный объект, отличающийся просторным жильем, отличной инфраструктурой и хорошо обустроенной территорией.\n\nForum Residence располагается на территории 316-го квартала Алматы. Данная местность сочетает динамический пульс современного мегаполиса и размеренное существование тихих двориков. ЖК находится на пересечении улиц Байтурсынова и Тимирязева. На расстоянии 500 метров располагается проспект Сакена Сейфуллина. В непосредственной близости от новостроек нет никаких промышленных объектов. До резиденции Президента всего 2 км, а до речки Есентай – не более 1000 метров.\n\nЖилой комплекс Forum Residence в Алматы имеет в своем составе шесть домов в 12 этажей, образующих уютный, защищенный от посторонних глаз двор.",
      kk: "Forum Residence ТҮК – кең тұрғын үй, тамаша инфрақұрылым және жақсы абаттандырылған аумақпен ерекшеленетін тартымды нысан.\n\nForum Residence Алматының 316-шы кварталының аумағында орналасқан. Бұл жер заманауи мегаполистің серпінді соғысы мен тыныш аулалардың баяу өмірін біріктіреді. ТҮК Байтұрсынов пен Тимирязев көшелерінің қиылысында орналасқан. 500 метр қашықтықта Сәкен Сейфуллин даңғылы орналасқан. Жаңа ғимараттардың жанында ешқандай өнеркәсіптік нысандар жоқ. Президент резиденциясына дейін бар болғаны 2 км, ал Есентай өзеніне дейін 1000 метрден аспайды.\n\nАлматыдағы Forum Residence тұрғын үй кешені жайлы, бөгде көздерден қорғалған аула құрайтын алты 12 қабатты үйден тұрады.",
      tr: "Forum Residence konut kompleksi – geniş konutları, mükemmel altyapısı ve iyi düzenlenmiş alanıyla öne çıkan çekici bir projedir.\n\nForum Residence, Almatı'nın 316. mahallesinde yer almaktadır. Bu bölge, modern megapolisin dinamik nabzını ve sessiz avluların sakin yaşamını bir arada sunmaktadır. Konut kompleksi Baytursynov ve Timiryazev sokaklarının kesişiminde bulunmaktadır. 500 metre mesafede Saken Seifullin bulvarı yer almaktadır. Yeni binaların yakınında hiçbir sanayi tesisi yoktur. Cumhurbaşkanlığı konutuna sadece 2 km, Esentay nehrine ise 1000 metreden fazla değildir.\n\nAlmatı'daki Forum Residence konut kompleksi, rahat ve meraklı gözlerden korunan bir avlu oluşturan altı adet 12 katlı binadan oluşmaktadır.",
      en: "Forum Residence residential complex is an attractive project distinguished by spacious housing, excellent infrastructure and well-maintained territory.\n\nForum Residence is located in the 316th quarter of Almaty. This area combines the dynamic pulse of a modern metropolis and the measured existence of quiet courtyards. The complex is located at the intersection of Baytursynov and Timiryazev streets. Saken Seifullin Avenue is located at a distance of 500 meters. There are no industrial facilities in the immediate vicinity of the new buildings. It is only 2 km to the Presidential residence, and no more than 1000 meters to the Esentay River.\n\nThe Forum Residence residential complex in Almaty consists of six 12-story buildings forming a cozy courtyard protected from prying eyes.",
    },
    status: { ru: "Сдан", kk: "Тапсырылды", tr: "Tamamlandı", en: "Completed" },
    features: [
      { ru: "Тихие дворы", kk: "Тыныш аулалар", tr: "Sessiz avlular", en: "Quiet courtyards" },
      { ru: "Вид на горы", kk: "Тауларға көрініс", tr: "Dağ manzarası", en: "Mountain view" },
      { ru: "Сейсмостойкость 10 баллов", kk: "10 балл сейсмикалық төзімділік", tr: "10 puan deprem dayanıklılığı", en: "Seismic resistance 10 points" },
      { ru: "Зоны отдыха, супермаркет, фитнес-центр", kk: "Демалыс аймақтары, супермаркет, фитнес-орталық", tr: "Dinlenme alanları, süpermarket, fitness merkezi", en: "Recreation areas, supermarket, fitness center" },
      { ru: "Бесшумные лифты", kk: "Дыбыссыз лифттер", tr: "Sessiz asansörler", en: "Silent elevators" },
    ],
  },

  // 22. ALMATY MUSEUM OF ARTS
  "22-almaty-museum": {
    title: { ru: "ALMATY MUSEUM OF ARTS", kk: "ALMATY MUSEUM OF ARTS", tr: "ALMATY MUSEUM OF ARTS", en: "ALMATY MUSEUM OF ARTS" },
    subtitle: { ru: "Новая точка на карте искусства", kk: "Өнер картасындағы жаңа нүкте", tr: "Sanat haritasında yeni bir nokta", en: "New point on the art map" },
    description: {
      ru: "Новая точка на карте современного искусства\nAlmaty Museum of Arts, основанный коллекционером Нурланом Смагуловым, первый музей такого рода в Казахстане.\n\nAlmaty Museum of Arts планирует насыщенную программу выставок, художественных проектов, издательскую деятельность и образовательные инициативы для детей и взрослых. Музей станет платформой для исследования и представления искусства Центральной Азии, способствуя обмену идеями и культурным диалогам.\n\nАрхитектурная составляющая музея и его окружающий ландшафт были спроектированы британским архитектурным бюро Chapman Taylor, мировым лидером в области архитектуры, градостроительства и интерьерного дизайна.\n\nДизайн музея вдохновлен природным ландшафтом Алматы и городской средой, который символизируют две «L»-образные структуры: одна из них олицетворяет горы, а другая представляет собой город.\n\nВ центре здания расположена «Улица искусства» — центральное пространство музея с высотой потолков 18 метров. Именно оно соединяет различные выставочные зоны. Окруженная естественным светом, «улица» вдохновлена величественным каньоном Чарын.",
      kk: "Қазіргі заманғы өнер картасындағы жаңа нүкте\nAlmaty Museum of Arts, коллекционер Нұрлан Смағұлов құрған, Қазақстандағы осындай түрдегі алғашқы мұражай.\n\nAlmaty Museum of Arts көрмелердің қанық бағдарламасын, көркем жобаларды, баспа қызметін және балалар мен ересектерге арналған білім беру бастамаларын жоспарлайды. Мұражай Орталық Азия өнерін зерттеу және таныстыру платформасына айналады, идеялар алмасуға және мәдени диалогтарға ықпал етеді.\n\nМұражайдың сәулеттік құрамдас бөлігі мен оның айналасындағы ландшафт сәулет, қала құрылысы және интерьер дизайны саласындағы әлемдік көшбасшы Chapman Taylor британдық сәулет бюросы жобалаған.\n\nМұражайдың дизайны Алматының табиғи ландшафты мен қалалық ортасынан шабыт алған, оны екі «L» тәрізді құрылым символдайды: біреуі тауларды бейнелейді, ал екіншісі қаланы білдіреді.\n\nҒимараттың орталығында «Өнер көшесі» орналасқан — төбе биіктігі 18 метр болатын мұражайдың орталық кеңістігі. Ол әртүрлі көрме аймақтарын байланыстырады.",
      tr: "Çağdaş sanat haritasında yeni bir nokta\nKoleksiyoner Nurlan Smagulov tarafından kurulan Almaty Museum of Arts, Kazakistan'da türünün ilk müzesidir.\n\nAlmaty Museum of Arts, yoğun bir sergi programı, sanat projeleri, yayıncılık faaliyetleri ve çocuklar ile yetişkinler için eğitim girişimleri planlamaktadır. Müze, Orta Asya sanatını araştırmak ve tanıtmak için bir platform olacak, fikir alışverişini ve kültürel diyalogları teşvik edecektir.\n\nMüzenin mimari bileşeni ve çevresindeki peyzaj, mimarlık, şehircilik ve iç tasarım alanında dünya lideri olan İngiliz mimarlık firması Chapman Taylor tarafından tasarlanmıştır.\n\nMüzenin tasarımı Almatı'nın doğal peyzajı ve kentsel ortamından ilham almış olup, iki «L» şeklindeki yapı ile sembolize edilmektedir: biri dağları, diğeri şehri temsil etmektedir.\n\nBinanın merkezinde, 18 metre tavan yüksekliğine sahip müzenin merkezi alanı olan «Sanat Sokağı» yer almaktadır. Farklı sergi alanlarını birbirine bağlamaktadır.",
      en: "A new point on the map of contemporary art\nAlmaty Museum of Arts, founded by collector Nurlan Smagulov, is the first museum of its kind in Kazakhstan.\n\nAlmaty Museum of Arts plans a rich program of exhibitions, art projects, publishing activities and educational initiatives for children and adults. The museum will become a platform for research and presentation of Central Asian art, promoting the exchange of ideas and cultural dialogues.\n\nThe architectural component of the museum and its surrounding landscape were designed by the British architectural firm Chapman Taylor, a world leader in architecture, urban planning and interior design.\n\nThe museum's design is inspired by Almaty's natural landscape and urban environment, symbolized by two «L»-shaped structures: one represents the mountains, and the other represents the city.\n\nIn the center of the building is the «Art Street» — the central space of the museum with a ceiling height of 18 meters. It connects various exhibition areas. Surrounded by natural light, the «street» is inspired by the majestic Charyn Canyon.",
    },
    status: { ru: "Завершен", kk: "Аяқталды", tr: "Tamamlandı", en: "Completed" },
    features: [
      { ru: "Первый музей такого рода в Казахстане", kk: "Қазақстандағы осындай түрдегі алғашқы мұражай", tr: "Kazakistan'da türünün ilk müzesi", en: "First museum of its kind in Kazakhstan" },
      { ru: "Спроектирован британским бюро Chapman Taylor", kk: "Chapman Taylor британдық бюросы жобалаған", tr: "İngiliz firması Chapman Taylor tarafından tasarlandı", en: "Designed by British firm Chapman Taylor" },
      { ru: "«Улица искусства» с потолками 18 метров", kk: "18 метр төбелі «Өнер көшесі»", tr: "18 metre tavanlı «Sanat Sokağı»", en: "«Art Street» with 18-meter ceilings" },
      { ru: "Вдохновлен каньоном Чарын", kk: "Шарын каньонынан шабыт алған", tr: "Şarın Kanyonu'ndan ilham almış", en: "Inspired by Charyn Canyon" },
      { ru: "Платформа для искусства Центральной Азии", kk: "Орталық Азия өнері үшін платформа", tr: "Orta Asya sanatı için platform", en: "Platform for Central Asian art" },
    ],
  },

  // 23. HAVAL АВТОЦЕНТР
  "23-haval": {
    title: { ru: "HAVAL АВТОЦЕНТР", kk: "HAVAL АВТООРТАЛЫҚ", tr: "HAVAL OTOMOBİL MERKEZİ", en: "HAVAL AUTO CENTER" },
    subtitle: { ru: "Мультибрендовый завод", kk: "Мультибрендтік зауыт", tr: "Çok markalı fabrika", en: "Multi-brand factory" },
    description: {
      ru: "Строительство мультибрендового завода по производству китайских автомобилей Chery, Haval и Changan.\n\nЗавод расположен в Индустриальной зоне Алматы, общая площадь его территории составляет 309 тыс. кв.м., площадь производственного комплекса — 211 000 кв.м.",
      kk: "Chery, Haval және Changan қытай автомобильдерін өндіру бойынша мультибрендтік зауыт құрылысы.\n\nЗауыт Алматының Индустриялық аймағында орналасқан, оның аумағының жалпы ауданы 309 мың шаршы метрді құрайды, өндірістік кешеннің ауданы — 211 000 шаршы метр.",
      tr: "Chery, Haval ve Changan Çin otomobillerinin üretimi için çok markalı fabrika inşaatı.\n\nFabrika Almatı Sanayi Bölgesi'nde yer almakta olup, toplam alan 309 bin metrekare, üretim kompleksinin alanı ise 211.000 metrekaredir.",
      en: "Construction of a multi-brand factory for the production of Chinese cars Chery, Haval and Changan.\n\nThe factory is located in the Industrial Zone of Almaty, its total area is 309 thousand sq.m., the area of the production complex is 211,000 sq.m.",
    },
    status: { ru: "Завершен", kk: "Аяқталды", tr: "Tamamlandı", en: "Completed" },
    features: [
      { ru: "Расположен в Индустриальной зоне Алматы", kk: "Алматының Индустриялық аймағында орналасқан", tr: "Almatı Sanayi Bölgesi'nde yer almaktadır", en: "Located in the Industrial Zone of Almaty" },
      { ru: "Общая площадь территории 309 тыс. кв.м.", kk: "Аумақтың жалпы ауданы 309 мың шаршы метр", tr: "Toplam alan 309 bin metrekare", en: "Total territory area 309 thousand sq.m." },
      { ru: "Площадь производственного комплекса 211 000 кв.м.", kk: "Өндірістік кешеннің ауданы 211 000 шаршы метр", tr: "Üretim kompleksinin alanı 211.000 metrekare", en: "Production complex area 211,000 sq.m." },
      { ru: "Производство Chery, Haval, Changan", kk: "Chery, Haval, Changan өндірісі", tr: "Chery, Haval, Changan üretimi", en: "Production of Chery, Haval, Changan" },
    ],
  },

  // 24. LUKOIL ЗАВОД
  "24-lukoil": {
    title: { ru: "LUKOIL ЗАВОД", kk: "LUKOIL ЗАУЫТЫ", tr: "LUKOIL FABRİKASI", en: "LUKOIL FACTORY" },
    subtitle: { ru: "Завод смазочных материалов", kk: "Майлау материалдары зауыты", tr: "Yağlama malzemeleri fabrikası", en: "Lubricants factory" },
    description: {
      ru: "Ключевым cтратегическим проектом в Казахстане стало строительство в Алматинской области современного завода по производству смазочных материалов. Строительство началось в мае 2016 года, а открытие завода состоялось в сентябре 2019 года.\n\nНовый завод в Казахстане стал самым передовым производственно-логистическим комплексом в регионе Центральной Азии. Его мощность составляет до 100 000 тонн продукции в год. Площадь производственной площадки – 12,5 га. На заводе работает роботизированный склад хранения готовой продукции и современная лаборатория контроля качества.\n\nЗавод расположен в 40 км от Алматы в Илийском районе, в непосредственной близости от международного автотранспортного коридора «Западная Европа – Западный Китай». Его географическое положение открывает Компании новые перспективы для эффективных поставок смазочных материалов в Китай и другие страны Азии.",
      kk: "Қазақстандағы негізгі стратегиялық жоба Алматы облысында майлау материалдарын өндіру бойынша заманауи зауыт құрылысы болды. Құрылыс 2016 жылдың мамырында басталды, ал зауыттың ашылуы 2019 жылдың қыркүйегінде болды.\n\nҚазақстандағы жаңа зауыт Орталық Азия аймағындағы ең озық өндірістік-логистикалық кешенге айналды. Оның қуаты жылына 100 000 тоннаға дейін өнім құрайды. Өндірістік алаңның ауданы – 12,5 га. Зауытта дайын өнімді сақтаудың роботтандырылған қоймасы және заманауи сапаны бақылау зертханасы жұмыс істейді.\n\nЗауыт Алматыдан 40 км қашықтықта Іле ауданында, «Батыс Еуропа – Батыс Қытай» халықаралық автокөлік дәлізіне жақын жерде орналасқан.",
      tr: "Kazakistan'daki kilit stratejik proje, Almatı bölgesinde yağlama malzemeleri üretimi için modern bir fabrika inşaatı oldu. İnşaat Mayıs 2016'da başladı ve fabrikanın açılışı Eylül 2019'da gerçekleşti.\n\nKazakistan'daki yeni fabrika, Orta Asya bölgesindeki en gelişmiş üretim ve lojistik kompleksi haline geldi. Kapasitesi yılda 100.000 tona kadardır. Üretim alanı 12,5 hektardır. Fabrikada robotik bitmiş ürün deposu ve modern kalite kontrol laboratuvarı bulunmaktadır.\n\nFabrika, Almatı'dan 40 km uzaklıkta İli bölgesinde, «Batı Avrupa – Batı Çin» uluslararası karayolu koridorunun yakınında yer almaktadır.",
      en: "The key strategic project in Kazakhstan was the construction of a modern lubricants factory in Almaty region. Construction began in May 2016, and the factory opened in September 2019.\n\nThe new factory in Kazakhstan has become the most advanced production and logistics complex in the Central Asian region. Its capacity is up to 100,000 tons of products per year. The production site area is 12.5 hectares. The factory has a robotic finished goods warehouse and a modern quality control laboratory.\n\nThe factory is located 40 km from Almaty in the Ili district, in close proximity to the Western Europe – Western China international road corridor. Its geographical location opens new prospects for efficient delivery of lubricants to China and other Asian countries.",
    },
    status: { ru: "Завершен", kk: "Аяқталды", tr: "Tamamlandı", en: "Completed" },
    features: [
      { ru: "Самый передовой производственно-логистический комплекс в Центральной Азии", kk: "Орталық Азиядағы ең озық өндірістік-логистикалық кешен", tr: "Orta Asya'daki en gelişmiş üretim ve lojistik kompleksi", en: "The most advanced production and logistics complex in Central Asia" },
      { ru: "Мощность до 100 000 тонн продукции в год", kk: "Жылына 100 000 тоннаға дейін өнім қуаты", tr: "Yılda 100.000 tona kadar kapasite", en: "Capacity up to 100,000 tons of products per year" },
      { ru: "Площадь производственной площадки 12,5 га", kk: "Өндірістік алаң ауданы 12,5 га", tr: "Üretim alanı 12,5 hektar", en: "Production site area 12.5 hectares" },
      { ru: "Роботизированный склад хранения готовой продукции", kk: "Дайын өнімді сақтаудың роботтандырылған қоймасы", tr: "Robotik bitmiş ürün deposu", en: "Robotic finished goods warehouse" },
      { ru: "Современная лаборатория контроля качества", kk: "Заманауи сапаны бақылау зертханасы", tr: "Modern kalite kontrol laboratuvarı", en: "Modern quality control laboratory" },
      { ru: "Расположен вблизи коридора «Западная Европа – Западный Китай»", kk: "«Батыс Еуропа – Батыс Қытай» дәлізіне жақын орналасқан", tr: "«Batı Avrupa – Batı Çin» koridoruna yakın konumda", en: "Located near the Western Europe – Western China corridor" },
    ],
  },

  // 25. BAITEREK SCHOOL
  "25-rams-city-school": {
    title: { ru: "BAITEREK SCHOOL", kk: "BAITEREK SCHOOL", tr: "BAITEREK SCHOOL", en: "BAITEREK SCHOOL" },
    subtitle: { ru: "Крупнейшая частная школа", kk: "Ең ірі жеке мектеп", tr: "En büyük özel okul", en: "Largest private school" },
    description: {
      ru: "Школа находится в жилом комплексе RAMS CITY в Алматы.\n\nBaiterek School of Science and Technologies\n\nКрупнейшая частная научно-технологическая школа Центральной Азии.\n\nBaiterek School — это новая школа, созданная как пространство науки, технологий и творчества.",
      kk: "Мектеп Алматыдағы RAMS CITY тұрғын үй кешенінде орналасқан.\n\nBaiterek School of Science and Technologies\n\nОрталық Азиядағы ең ірі жеке ғылыми-технологиялық мектеп.\n\nBaiterek School — ғылым, технология және шығармашылық кеңістігі ретінде құрылған жаңа мектеп.",
      tr: "Okul, Almatı'daki RAMS CITY konut kompleksinde yer almaktadır.\n\nBaiterek School of Science and Technologies\n\nOrta Asya'nın en büyük özel bilim ve teknoloji okulu.\n\nBaiterek School — bilim, teknoloji ve yaratıcılık alanı olarak oluşturulmuş yeni bir okuldur.",
      en: "The school is located in the RAMS CITY residential complex in Almaty.\n\nBaiterek School of Science and Technologies\n\nThe largest private science and technology school in Central Asia.\n\nBaiterek School is a new school created as a space of science, technology and creativity.",
    },
    status: { ru: "Завершен", kk: "Аяқталды", tr: "Tamamlandı", en: "Completed" },
    features: [
      { ru: "Локация: Алматы, Бостандыкский район", kk: "Орналасуы: Алматы, Бостандық ауданы", tr: "Konum: Almatı, Bostanık bölgesi", en: "Location: Almaty, Bostandyk district" },
      { ru: "Дата открытия: 1 сентября 2025 года", kk: "Ашылу күні: 2025 жылдың 1 қыркүйегі", tr: "Açılış tarihi: 1 Eylül 2025", en: "Opening date: September 1, 2025" },
      { ru: "Крупнейшая частная школа Центральной Азии", kk: "Орталық Азиядағы ең ірі жеке мектеп", tr: "Orta Asya'nın en büyük özel okulu", en: "The largest private school in Central Asia" },
      { ru: "Площадь кампуса: около 28 000 м²", kk: "Кампус ауданы: шамамен 28 000 м²", tr: "Kampüs alanı: yaklaşık 28.000 m²", en: "Campus area: approximately 28,000 m²" },
      { ru: "Вместимость: до 3 000 учеников", kk: "Сыйымдылығы: 3 000 оқушыға дейін", tr: "Kapasite: 3.000 öğrenciye kadar", en: "Capacity: up to 3,000 students" },
      { ru: "Формат обучения: полный день, с 08:00 до 17:00", kk: "Оқыту форматы: толық күн, 08:00-ден 17:00-ге дейін", tr: "Eğitim formatı: tam gün, 08:00-17:00", en: "Learning format: full day, from 08:00 to 17:00" },
      { ru: "Модель обучения: факультетская система, проектно-исследовательский подход, международные стандарты", kk: "Оқыту моделі: факультеттік жүйе, жобалық-зерттеу тәсілі, халықаралық стандарттар", tr: "Eğitim modeli: fakülte sistemi, proje-araştırma yaklaşımı, uluslararası standartlar", en: "Learning model: faculty system, project-research approach, international standards" },
    ],
  },

  // 26. MARRIOTT ISSYK-KUL RESORT
  "26-marriott-issykkul": {
    title: { ru: "MARRIOTT ISSYK-KUL", kk: "MARRIOTT ISSYK-KUL", tr: "MARRIOTT ISSIK-KUL", en: "MARRIOTT ISSYK-KUL" },
    subtitle: { ru: "Элитный курорт на Иссык-Куле", kk: "Ыстықкөлдегі элит курорт", tr: "Issık-Kul'da elit tatil köyü", en: "Elite resort at Issyk-Kul" },
    description: {
      ru: "Rams Resort & Villas — элитный курортный комплекс на берегу Иссык-Куля, предлагающий сочетание пятизвёздочного гостиничного сервиса и приватной жизни в виллах. В комплексе всего 33 приватные виллы, что подчеркивает статус эксклюзивности, уединённости и высокий уровень отдыха.\n\nПроект включает отель под брендом Marriott — с историей, репутацией и сервисом мирового класса. Виллы оснащены современными удобствами: дизайнерской отделкой, высокими потолками, просторными террасами и балконами, закрытыми кухнями с эргономикой и встроенной техникой, гардеробными. Также в проект входят private марина для яхт, спортивные площадки, корты (паддел-теннис, волейбол, футбол, баскетбол), что делает отдых активным и разнообразным на фоне горных пейзажей и природной красоты.\n\nЛокация Rams Resort & Villas — северное побережье Иссык-Куля, в городе Чолпон-Ата — регион с нарастающим туристическим потенциалом.",
      kk: "Rams Resort & Villas — Ыстықкөл жағасындағы элиталық курорттық кешен, бес жұлдызды қонақ үй қызметі мен виллалардағы жеке өмірді ұсынады. Кешенде барлығы 33 жеке вилла бар, бұл эксклюзивтілік, оңашалық мәртебесін және демалыстың жоғары деңгейін көрсетеді.\n\nЖоба тарихы, беделі және әлемдік деңгейдегі қызметі бар Marriott брендіндегі қонақ үйді қамтиды. Виллалар заманауи ыңғайлылықтармен жабдықталған: дизайнерлік өңдеу, биік төбелер, кең террасалар мен балкондар, эргономикасы мен кірістірілген техникасы бар жабық асханалар, гардеробтар. Сондай-ақ жобаға яхталарға арналған private марина, спорт алаңдары, корттар (паддел-теннис, волейбол, футбол, баскетбол) кіреді.\n\nRams Resort & Villas орналасуы — Ыстықкөлдің солтүстік жағалауы, Чолпон-Ата қаласы — туристік әлеуеті өсіп келе жатқан аймақ.",
      tr: "Rams Resort & Villas — Issık-Kul kıyısında beş yıldızlı otel hizmeti ile villalarda özel yaşamı bir arada sunan elit bir tatil köyü kompleksidir. Komplekste toplam 33 özel villa bulunmakta olup, bu durum münhasırlık, mahremiyet statüsünü ve yüksek tatil seviyesini vurgulamaktadır.\n\nProje, tarihi, itibarı ve dünya standartlarında hizmeti olan Marriott markası altında bir otel içermektedir. Villalar modern olanaklarla donatılmıştır: tasarımcı dekorasyonu, yüksek tavanlar, geniş teraslar ve balkonlar, ergonomi ve ankastre cihazlara sahip kapalı mutfaklar, giyinme odaları. Ayrıca projede yatlar için özel marina, spor alanları, kortlar (padel tenisi, voleybol, futbol, basketbol) bulunmaktadır.\n\nRams Resort & Villas konumu — Issık-Kul'un kuzey kıyısı, Çolpon-Ata şehri — turizm potansiyeli artan bir bölge.",
      en: "Rams Resort & Villas is an elite resort complex on the shores of Issyk-Kul, offering a combination of five-star hotel service and private life in villas. The complex has only 33 private villas, which emphasizes the status of exclusivity, privacy and a high level of recreation.\n\nThe project includes a Marriott-branded hotel with a history, reputation and world-class service. The villas are equipped with modern amenities: designer finishes, high ceilings, spacious terraces and balconies, closed kitchens with ergonomics and built-in appliances, dressing rooms. The project also includes a private marina for yachts, sports grounds, courts (paddle tennis, volleyball, football, basketball), which makes recreation active and varied against the backdrop of mountain landscapes and natural beauty.\n\nThe location of Rams Resort & Villas is the northern coast of Issyk-Kul, in the city of Cholpon-Ata — a region with growing tourism potential.",
    },
    status: { ru: "Строится", kk: "Салынуда", tr: "İnşaat Halinde", en: "Under Construction" },
    features: [
      { ru: "Эксклюзивность и приватность — всего 33 виллы", kk: "Эксклюзивтілік және жекелік — барлығы 33 вилла", tr: "Münhasırlık ve mahremiyet — toplam 33 villa", en: "Exclusivity and privacy — only 33 villas" },
      { ru: "Мировой бренд Marriott", kk: "Marriott әлемдік бренді", tr: "Dünya markası Marriott", en: "World brand Marriott" },
      { ru: "Private марина для яхт", kk: "Яхталарға арналған private марина", tr: "Yatlar için özel marina", en: "Private marina for yachts" },
      { ru: "Спортивные площадки: паддел-теннис, волейбол, футбол, баскетбол", kk: "Спорт алаңдары: паддел-теннис, волейбол, футбол, баскетбол", tr: "Spor alanları: padel tenisi, voleybol, futbol, basketbol", en: "Sports grounds: paddle tennis, volleyball, football, basketball" },
      { ru: "Дизайнерская отделка, высокие потолки, просторные террасы", kk: "Дизайнерлік өңдеу, биік төбелер, кең террасалар", tr: "Tasarımcı dekorasyonu, yüksek tavanlar, geniş teraslar", en: "Designer finishes, high ceilings, spacious terraces" },
      { ru: "Инвестиционная привлекательность — срок окупаемости до 9 лет", kk: "Инвестициялық тартымдылық — өтелу мерзімі 9 жылға дейін", tr: "Yatırım çekiciliği — geri ödeme süresi 9 yıla kadar", en: "Investment attractiveness — payback period up to 9 years" },
      { ru: "Facility Services от RAMS Global: охрана 24/7, обслуживание, консьерж-услуги", kk: "RAMS Global-дан Facility Services: 24/7 күзет, қызмет көрсету, консьерж-қызметтер", tr: "RAMS Global'dan Facility Services: 7/24 güvenlik, hizmet, concierge hizmetleri", en: "Facility Services from RAMS Global: 24/7 security, maintenance, concierge services" },
    ],
  },

  // 27. HYUNDAI TRANS KAZAKHSTAN
  "27-hyundai": {
    title: { ru: "HYUNDAI TRANS KAZAKHSTAN", kk: "HYUNDAI TRANS KAZAKHSTAN", tr: "HYUNDAI TRANS KAZAKHSTAN", en: "HYUNDAI TRANS KAZAKHSTAN" },
    subtitle: { ru: "Завод легковых автомобилей", kk: "Жеңіл автомобильдер зауыты", tr: "Binek otomobil fabrikası", en: "Passenger car factory" },
    description: {
      ru: "В Алматы Hyundai Trans Kazakhstan (производство легковых автомобилей). Запущен в 2020 году.\n\nЗавод по производству легковых автомобилей Hyundai Trans Kazakhstan расположен на 15 гектарах земли в Индустриальной зоне Алматы. Строительство начали летом 2019 года. Завод состоит из шести корпусов. На территории завода будут лишь сварочный и покрасочный цеха, а также цех сборки, где будут укомплектовываться автомобили. Общая площадь мероприятия 44 000 кв.м. Завод Hyundai Trans Kazakhstan построен с применением современных технологий мирового автомобилестроения и соответствует требованиям экологических стандартов, оснащен новейшим оборудованием производства Южной Кореи.",
      kk: "Алматыда Hyundai Trans Kazakhstan (жеңіл автомобильдер өндірісі). 2020 жылы іске қосылды.\n\nHyundai Trans Kazakhstan жеңіл автомобильдер зауыты Алматының Индустриялық аймағында 15 гектар жерде орналасқан. Құрылыс 2019 жылдың жазында басталды. Зауыт алты корпустан тұрады. Зауыт аумағында тек пісіру және бояу цехтары, сондай-ақ автомобильдер жиналатын құрастыру цехы болады. Жалпы ауданы 44 000 шаршы метр. Hyundai Trans Kazakhstan зауыты әлемдік автомобиль жасаудың заманауи технологияларын қолдана отырып салынған және экологиялық стандарттар талаптарына сай келеді, Оңтүстік Кореяда өндірілген ең жаңа жабдықтармен жабдықталған.",
      tr: "Almatı'da Hyundai Trans Kazakhstan (binek otomobil üretimi). 2020 yılında faaliyete geçmiştir.\n\nHyundai Trans Kazakhstan binek otomobil fabrikası, Almatı Sanayi Bölgesi'nde 15 hektarlık bir arazide yer almaktadır. İnşaat 2019 yazında başlamıştır. Fabrika altı binadan oluşmaktadır. Fabrika alanında yalnızca kaynak ve boya atölyeleri ile otomobillerin monte edileceği montaj atölyesi bulunacaktır. Toplam alan 44.000 m²'dir. Hyundai Trans Kazakhstan fabrikası, dünya otomotiv endüstrisinin modern teknolojileri kullanılarak inşa edilmiş ve çevre standartlarının gereksinimlerini karşılamakta, Güney Kore üretimi en yeni ekipmanlarla donatılmıştır.",
      en: "In Almaty Hyundai Trans Kazakhstan (passenger car production). Launched in 2020.\n\nThe Hyundai Trans Kazakhstan passenger car factory is located on 15 hectares of land in the Industrial Zone of Almaty. Construction began in the summer of 2019. The factory consists of six buildings. On the territory of the factory there will only be welding and painting workshops, as well as an assembly workshop where cars will be assembled. The total area is 44,000 sq.m. The Hyundai Trans Kazakhstan factory is built using modern technologies of world automobile manufacturing and meets the requirements of environmental standards, equipped with the latest equipment produced in South Korea.",
    },
    status: { ru: "Завершен", kk: "Аяқталды", tr: "Tamamlandı", en: "Completed" },
    features: [
      { ru: "Расположен на 15 гектарах в Индустриальной зоне Алматы", kk: "Алматының Индустриялық аймағында 15 гектарда орналасқан", tr: "Almatı Sanayi Bölgesi'nde 15 hektarda yer almaktadır", en: "Located on 15 hectares in the Industrial Zone of Almaty" },
      { ru: "Завод состоит из шести корпусов", kk: "Зауыт алты корпустан тұрады", tr: "Fabrika altı binadan oluşmaktadır", en: "The factory consists of six buildings" },
      { ru: "Общая площадь 44 000 кв.м.", kk: "Жалпы ауданы 44 000 шаршы метр", tr: "Toplam alan 44.000 m²", en: "Total area 44,000 sq.m." },
      { ru: "Сварочный, покрасочный цеха и цех сборки", kk: "Пісіру, бояу цехтары және құрастыру цехы", tr: "Kaynak, boya atölyeleri ve montaj atölyesi", en: "Welding, painting workshops and assembly workshop" },
      { ru: "Построен с применением современных технологий мирового автомобилестроения", kk: "Әлемдік автомобиль жасаудың заманауи технологияларымен салынған", tr: "Dünya otomotiv endüstrisinin modern teknolojileri kullanılarak inşa edilmiştir", en: "Built using modern technologies of world automobile manufacturing" },
      { ru: "Соответствует экологическим стандартам", kk: "Экологиялық стандарттарға сай келеді", tr: "Çevre standartlarını karşılamaktadır", en: "Meets environmental standards" },
      { ru: "Оснащен новейшим оборудованием из Южной Кореи", kk: "Оңтүстік Кореядан ең жаңа жабдықтармен жабдықталған", tr: "Güney Kore'den en yeni ekipmanlarla donatılmıştır", en: "Equipped with the latest equipment from South Korea" },
    ],
  },

  // 28. ЖК SAKURA
  "28-sakura": {
    title: { ru: "ЖК SAKURA", kk: "SAKURA ТҮК", tr: "SAKURA KONUT KOMPLEKSİ", en: "SAKURA RESIDENTIAL COMPLEX" },
    subtitle: { ru: "Японский минимализм у гор", kk: "Таулар жанындағы жапон минимализмі", tr: "Dağlarda Japon minimalizmi", en: "Japanese minimalism at the mountains" },
    description: {
      ru: "Современный жилой комплекс «Sakura» находится около ущелья Ремизовка. Жилой комплекс с лаконичной архитектурой, в котором соединяется японская четкость и сдержанность. Минималистичный стиль архитектуры жилых домов отлично сочетается с живописным ландшафтом. Современный ЖК Sakura в Алматы насчитывает 13 жилых домов, это аккуратные восьмиэтажные строения с панорамным остеклением и прекрасными видами на ущелье.\n\nЖилой комплекс Sakura — это сочетание природы, уюта и современного комфорта. Комплекс расположен на живописной закрытой территории площадью семь гектаров, где создано всё для спокойной и размеренной жизни. Здесь предусмотрены два собственных бассейна, зона для пикников и барбекю, крытый паркинг с местом для каждого жильца. Высота потолков составляет 3,3 метра, что создаёт ощущение простора и свободы.\n\nЖК отличается сейсмоустойчивостью до десяти баллов и предлагает своим жителям атмосферу элитного соседства, чистый горный воздух и гармонию с природой прямо у дома.",
      kk: "Заманауи «Sakura» тұрғын үй кешені Ремизов шатқалының жанында орналасқан. Жапондық нақтылық пен ұстамдылықты біріктіретін қарапайым сәулеті бар тұрғын үй кешені. Тұрғын үйлердің минималистік сәулет стилі көрікті ландшафтпен тамаша үйлеседі. Алматыдағы заманауи Sakura ТҮК 13 тұрғын үйден тұрады, бұлар панорамалық әйнектеуі және шатқалға керемет көріністері бар ұқыпты сегіз қабатты ғимараттар.\n\nSakura тұрғын үй кешені — табиғат, жайлылық және заманауи комфорттың үйлесімі. Кешен жеті гектар көрікті жабық аумақта орналасқан, мұнда тыныш және баяу өмір сүру үшін бәрі жасалған. Мұнда екі жеке бассейн, пикник және барбекю аймағы, әрбір тұрғынға орын бар жабық паркинг қарастырылған. Төбе биіктігі 3,3 метр құрайды, бұл кеңістік пен еркіндік сезімін тудырады.\n\nТҮК он балға дейін сейсмикалық төзімділігімен ерекшеленеді және тұрғындарына элиталық көршілік атмосферасын, таза тау ауасын және үйдің жанында табиғатпен үйлесімділікті ұсынады.",
      tr: "Modern «Sakura» konut kompleksi Remizovka vadisinin yakınında yer almaktadır. Japon netliği ve ölçülülüğünü birleştiren yalın mimariye sahip konut kompleksi. Konut binalarının minimalist mimari tarzı, pitoresk peyzajla mükemmel bir şekilde uyum sağlamaktadır. Almatı'daki modern Sakura konut kompleksi 13 konut binasından oluşmakta olup, bunlar panoramik camları ve vadiye harika manzaraları olan düzgün sekiz katlı yapılardır.\n\nSakura konut kompleksi — doğa, konfor ve modern rahatlığın birleşimidir. Kompleks, sakin ve ölçülü bir yaşam için her şeyin yaratıldığı yedi hektarlık pitoresk kapalı bir alanda yer almaktadır. Burada iki özel havuz, piknik ve barbekü alanı, her sakine yer ayrılmış kapalı otopark bulunmaktadır. Tavan yüksekliği 3,3 metredir, bu da ferahlık ve özgürlük hissi yaratmaktadır.\n\nKonut kompleksi on puana kadar deprem dayanıklılığıyla öne çıkmakta ve sakinlerine elit komşuluk atmosferi, temiz dağ havası ve evin hemen yanında doğayla uyum sunmaktadır.",
      en: "The modern «Sakura» residential complex is located near the Remizovka gorge. A residential complex with laconic architecture combining Japanese precision and restraint. The minimalist architectural style of residential buildings perfectly matches the picturesque landscape. The modern Sakura residential complex in Almaty has 13 residential buildings, these are neat eight-story structures with panoramic glazing and beautiful views of the gorge.\n\nThe Sakura residential complex is a combination of nature, coziness and modern comfort. The complex is located on a picturesque closed territory of seven hectares, where everything has been created for a calm and measured life. There are two private pools, a picnic and barbecue area, covered parking with a space for each resident. The ceiling height is 3.3 meters, which creates a feeling of spaciousness and freedom.\n\nThe complex is distinguished by seismic resistance up to ten points and offers its residents an atmosphere of elite neighborhood, clean mountain air and harmony with nature right at home.",
    },
    status: { ru: "Сдан", kk: "Тапсырылды", tr: "Tamamlandı", en: "Completed" },
    features: [
      { ru: "Живописная закрытая территория площадью 7 гектаров", kk: "7 гектар көрікті жабық аумақ", tr: "7 hektarlık pitoresk kapalı alan", en: "Picturesque closed territory of 7 hectares" },
      { ru: "Два собственных бассейна", kk: "Екі жеке бассейн", tr: "İki özel havuz", en: "Two private pools" },
      { ru: "Зона для пикников и барбекю", kk: "Пикник және барбекю аймағы", tr: "Piknik ve barbekü alanı", en: "Picnic and barbecue area" },
      { ru: "Крытый паркинг с местом для каждого жильца", kk: "Әрбір тұрғынға орын бар жабық паркинг", tr: "Her sakine yer ayrılmış kapalı otopark", en: "Covered parking with a space for each resident" },
      { ru: "Высота потолков 3,3 метра", kk: "Төбе биіктігі 3,3 метр", tr: "Tavan yüksekliği 3,3 metre", en: "Ceiling height 3.3 meters" },
      { ru: "Сейсмоустойчивость до 10 баллов", kk: "10 балға дейін сейсмикалық төзімділік", tr: "10 puana kadar deprem dayanıklılığı", en: "Seismic resistance up to 10 points" },
      { ru: "13 жилых домов с панорамным остеклением", kk: "Панорамалық әйнектеуі бар 13 тұрғын үй", tr: "Panoramik camlı 13 konut binası", en: "13 residential buildings with panoramic glazing" },
      { ru: "Атмосфера элитного соседства", kk: "Элиталық көршілік атмосферасы", tr: "Elit komşuluk atmosferi", en: "Elite neighborhood atmosphere" },
      { ru: "Чистый горный воздух", kk: "Таза тау ауасы", tr: "Temiz dağ havası", en: "Clean mountain air" },
    ],
  },
};

// Helper function to get localized project data
export function getLocalizedProject(projectId: string, language: Language) {
  const translation = projectTranslations[projectId];
  if (!translation) return null;

  return {
    title: translation.title[language],
    subtitle: translation.subtitle[language],
    description: translation.description[language],
    status: translation.status[language],
    features: translation.features.map(f => f[language]),
  };
}

// Helper to get localized status
export function getLocalizedStatus(status: string, language: Language): string {
  return statusTranslations[status]?.[language] || status;
}

// Helper to get localized scene type
export function getLocalizedSceneType(type: string, language: Language): string {
  return sceneTypeTranslations[type]?.[language] || type;
}

// Helper to get localized scene title
export function getLocalizedSceneTitle(title: string, language: Language): string {
  if (sceneTitleTranslations[title]) {
    return sceneTitleTranslations[title][language];
  }

  if (title.startsWith("Вид ")) {
    const viewLabels: Record<Language, string> = {
      ru: "Вид",
      kk: "Көрініс",
      tr: "Görünüm",
      en: "View",
    };
    const num = title.replace("Вид ", "");
    return `${viewLabels[language]} ${num}`;
  }

  return title;
}

// Helper to get localized property class
export function getLocalizedClass(propertyClass: string, language: Language): string {
  return classTranslations[propertyClass]?.[language] || propertyClass;
}

// Helper to check if project is under construction
export function isProjectUnderConstruction(status: string): boolean {
  return status === "Строится" || status.includes("очередь");
}

// Helper to get localized quarter
export function getLocalizedQuarter(quarter: string | undefined, language: Language): string {
  if (!quarter) return "";
  return quarterTranslations[quarter]?.[language] || quarter;
}

// Helper to get localized location label
export function getLocalizedLocation(label: string, language: Language): string {
  return locationTranslations[label]?.[language] || label;
}

// Helper to localize measurements (converts м to m, км to km based on language)
export function getLocalizedMeasurement(value: string, language: Language): string {
  if (!value) return "";
  const units = unitTranslations[language];
  return value
    .replace(/(\d+(?:[.,]\d+)?)\s*м(?![а-яa-z])/gi, `$1${units.meters}`)
    .replace(/(\d+(?:[.,]\d+)?)\s*км/gi, `$1 ${units.km}`);
}

// Helper to get localized ceiling height
export function getLocalizedCeilingHeight(height: string, language: Language): string {
  return getLocalizedMeasurement(height, language);
}

// Helper to get localized distance
export function getLocalizedDistance(distance: string, language: Language): string {
  return getLocalizedMeasurement(distance, language);
}
