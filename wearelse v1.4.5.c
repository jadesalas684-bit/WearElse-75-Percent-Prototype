#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_ITEMS 100
#define SIZE 50

typedef struct {
    char name[SIZE], category[20], color[SIZE], footwearType[SIZE], occasion[20];
} Clothing;

Clothing actual[MAX_ITEMS], generated[MAX_ITEMS];
int actualCount = 0, generatedCount = 0;

void getText(const char *prompt, char *out, int size) {
    int ch; size_t n;
    printf("%s", prompt);
    if (!fgets(out, size, stdin)) { out[0] = '\0'; return; }
    n = strlen(out);
    if (n && out[n - 1] == '\n') out[n - 1] = '\0';
    else while ((ch = getchar()) != '\n' && ch != EOF) {}
}

int getNumber(const char *prompt) {
    char s[30], *end; long value;
    while (1) {
        getText(prompt, s, sizeof(s));
        value = strtol(s, &end, 10);
        while (isspace((unsigned char)*end)) end++;
        if (end != s && *end == '\0') return (int)value;
        puts("Please enter a valid number.");
    }
}

int equalText(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
        a++; b++;
    }
    return *a == '\0' && *b == '\0';
}

void showWardrobe(Clothing list[], int count) {
    int i;
    if (!count) { puts("No clothing items available."); return; }
    for (i = 0; i < count; i++) {
        printf("\n%d. %s | %s | %s | Occasion: %s", i + 1,
               list[i].name, list[i].category, list[i].color, list[i].occasion);
        if (equalText(list[i].category, "FOOTWEAR"))
            printf(" | Type: %s", list[i].footwearType);
        putchar('\n');
    }
}

void addClothes(void) {
    Clothing item; int c, o;
    while (1) {
        char answer[8];
        if (actualCount >= MAX_ITEMS) { puts("Wardrobe is full (100 items)."); return; }
        getText("Clothing name: ", item.name, SIZE);
        puts("Category: 1.Top  2.Bottom  3.Footwear");
        c = getNumber("Choose: ");
        if (c == 1) { strcpy(item.category, "TOP"); strcpy(item.footwearType, "N/A"); }
        else if (c == 2) { strcpy(item.category, "BOTTOM"); strcpy(item.footwearType, "N/A"); }
        else if (c == 3) {
            strcpy(item.category, "FOOTWEAR");
            getText("Footwear type: ", item.footwearType, SIZE);
        } else { puts("Invalid category."); return; }
        getText("Color: ", item.color, SIZE);
        puts("Occasion: 1.School 2.Casual 3.Formal 4.Outdoor 5.Party 6.Any");
        o = getNumber("Choose: ");
        switch (o) {
            case 1: strcpy(item.occasion, "School"); break;
            case 2: strcpy(item.occasion, "Casual"); break;
            case 3: strcpy(item.occasion, "Formal"); break;
            case 4: strcpy(item.occasion, "Outdoor"); break;
            case 5: strcpy(item.occasion, "Party"); break;
            case 6: strcpy(item.occasion, "Any"); break;
            default: puts("Invalid occasion."); return;
        }
        actual[actualCount++] = item;
        puts("Clothing item added.");
        if (actualCount >= MAX_ITEMS) { puts("Wardrobe is full (100 items)."); return; }
        getText("Add another? (y/n): ", answer, sizeof(answer));
        if (!equalText(answer, "y") && !equalText(answer, "yes")) break;
    }
}

void removeClothes(void) {
    int n, i;
    showWardrobe(actual, actualCount);
    if (!actualCount) return;
    n = getNumber("Enter item number to remove: ");
    if (n < 1 || n > actualCount) { puts("Invalid item number."); return; }
    for (i = n - 1; i < actualCount - 1; i++) actual[i] = actual[i + 1];
    actualCount--; puts("Item removed.");
}

void makeRandomWardrobe(void) {
    const char *tops[] = {"T-shirt", "Polo Shirt", "Hoodie", "Blouse", "Button-down"};
    const char *bottoms[] = {"Jeans", "Chinos", "Shorts", "Trousers", "Joggers"};
    const char *shoes[] = {"Sneakers", "Loafers", "Sandals", "Running Shoes", "Slip-ons"};
    const char *colors[] = {"Black", "White", "Blue", "Gray", "Beige", "Brown", "Red", "Green"};
    int i;
    generatedCount = 0;
    for (i = 0; i < 15; i++) {
        Clothing *x = &generated[generatedCount];
        if (i < 5) { strcpy(x->name, tops[i]); strcpy(x->category, "TOP"); strcpy(x->footwearType, "N/A"); }
        else if (i < 10) { strcpy(x->name, bottoms[i-5]); strcpy(x->category, "BOTTOM"); strcpy(x->footwearType, "N/A"); }
        else { strcpy(x->name, shoes[i-10]); strcpy(x->category, "FOOTWEAR"); strcpy(x->footwearType, shoes[i-10]); }
        strcpy(x->color, colors[rand() % 8]);
        strcpy(x->occasion, "Any");
        generatedCount++;
    }
    puts("Random wardrobe generated: 5 tops, 5 bottoms, 5 footwear.");
}

int neutral(const char *c) {
    return equalText(c,"Black") || equalText(c,"White") || equalText(c,"Gray") || equalText(c,"Beige") || equalText(c,"Brown");
}
int dark(const char *c) {
    return equalText(c,"Black") || equalText(c,"Blue") || equalText(c,"Gray") || equalText(c,"Brown");
}
int light(const char *c) {
    return equalText(c,"White") || equalText(c,"Beige") || equalText(c,"Gray");
}
int colorOK(const char *a, const char *b, const char *c, int scheme) {
    if (scheme == 1) return 1;
    if (scheme == 2) return neutral(a) && neutral(b) && neutral(c);
    if (scheme == 3) return equalText(a,b) && equalText(a,c);
    if (scheme == 4) return dark(a) && dark(b) && dark(c);
    if (scheme == 5) return light(a) && light(b) && light(c);
    return 0;
}
int occasionOK(const Clothing *x, const char *occasion) {
    return equalText(x->occasion,"Any") || equalText(x->occasion,occasion);
}

void recommend(Clothing list[], int count) {
    const char *occasions[] = {"School","Casual","Formal","Outdoor","Party"};
    const char *schemes[] = {"Any Color Combination","Neutral Colors","Monochromatic","Dark Tones","Light Tones"};
    int o, s, i, j, k, total = 0, target, seen = 0, ti = -1, bi = -1, fi = -1;
    if (!count) { puts("This wardrobe is empty. Add or generate clothes first."); return; }
    puts("Occasion: 1.School 2.Casual 3.Formal 4.Outdoor 5.Party");
    o = getNumber("Choose: ");
    puts("Color scheme: 1.Any 2.Neutral 3.Monochromatic 4.Dark 5.Light");
    s = getNumber("Choose: ");
    if (o < 1 || o > 5 || s < 1 || s > 5) { puts("Invalid selection."); return; }

    /* Count combinations that pass the category, occasion, and color rules. */
    for (i = 0; i < count; i++) if (equalText(list[i].category,"TOP") && occasionOK(&list[i],occasions[o-1]))
      for (j = 0; j < count; j++) if (equalText(list[j].category,"BOTTOM") && occasionOK(&list[j],occasions[o-1]))
        for (k = 0; k < count; k++) if (equalText(list[k].category,"FOOTWEAR") && occasionOK(&list[k],occasions[o-1]))
          if (colorOK(list[i].color,list[j].color,list[k].color,s)) total++;

    if (!total) { puts("No valid outfit found. Try different preferences or add more clothes."); return; }
    target = rand() % total;
    for (i = 0; i < count; i++) if (equalText(list[i].category,"TOP") && occasionOK(&list[i],occasions[o-1]))
      for (j = 0; j < count; j++) if (equalText(list[j].category,"BOTTOM") && occasionOK(&list[j],occasions[o-1]))
        for (k = 0; k < count; k++) if (equalText(list[k].category,"FOOTWEAR") && occasionOK(&list[k],occasions[o-1]))
          if (colorOK(list[i].color,list[j].color,list[k].color,s)) {
              if (seen++ == target) { ti = i; bi = j; fi = k; }
          }
    printf("\n--- RECOMMENDED OUTFIT ---\nOccasion: %s\nColor scheme: %s\n", occasions[o-1], schemes[s-1]);
    printf("Top: %s (%s)\nBottom: %s (%s)\nFootwear: %s (%s)\n",
           list[ti].name,list[ti].color,list[bi].name,list[bi].color,list[fi].name,list[fi].color);
    printf("Valid combinations found: %d\n", total);
}

int main(void) {
    int choice;
    srand((unsigned int)time(NULL));
    do {
        puts("\n========== WEARELSE ==========");
        puts("1. My Actual Wardrobe");
        puts("2. Generate a Random Wardrobe");
        puts("3. Surprise Me! (Actual Clothes)");
        puts("4. Surprise Me! (Random Clothes)");
        puts("0. Exit");
        choice = getNumber("Enter choice: ");
        switch (choice) {
            case 1: {
                int sub;
                do {
                    printf("\n--- ACTUAL WARDROBE (%d/%d) ---\n", actualCount, MAX_ITEMS);
                    puts("1. Add  2. View  3. Remove  4. Delete all  0. Back");
                    sub = getNumber("Choose: ");
                    if (sub == 1) addClothes();
                    else if (sub == 2) showWardrobe(actual, actualCount);
                    else if (sub == 3) removeClothes();
                    else if (sub == 4) {
                        char a[8];
                        getText("Delete all items? (y/n): ", a, sizeof(a));
                        if (equalText(a,"y") || equalText(a,"yes")) { actualCount = 0; puts("Wardrobe cleared."); }
                    } else if (sub != 0) puts("Invalid choice.");
                } while (sub != 0);
                break;
            }
            case 2: makeRandomWardrobe(); showWardrobe(generated, generatedCount); break;
            case 3: recommend(actual, actualCount); break;
            case 4:
                if (!generatedCount) puts("Generate the random wardrobe first.");
                else recommend(generated, generatedCount);
                break;
            case 0: puts("Thank you for using WearElse. Data resets when the program closes."); break;
            default: puts("Invalid choice.");
        }
    } while (choice != 0);
    return 0;
}
