/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "sysinfo/keyboard.h"

#include <libintl.h>
#include <locale.h>
#include <QDebug>
#include <QDomDocument>

#include "base/command.h"
#include "base/consts.h"
#include "base/file_util.h"

namespace installer {

namespace {

// Domain name of xkb-data.
const char kXkbDomain[] = "xkeyboard-config";

const char kSetXkbMapCmd[] = "/usr/bin/setxkbmap";

const char kXkbBaseRule[] = "/usr/share/X11/xkb/rules/base.xml";
const char kXkbExtraRule[] = "/usr/share/X11/xkb/rules/base.extras.xml";

// Get localized |description|.
QString GetLocalDesc(const QString& description) {
  return QString(dgettext(kXkbDomain, description.toLocal8Bit().constData()));
}

// Read modelList node.
void ReadModelList(const QDomNode& root, XkbModelList& model_list) {
  const QDomNodeList model_nodes(root.childNodes());
  for (int index = 0; index < model_nodes.length(); index++) {
    XkbModel xkb_model;
    const QDomNodeList config_nodes(model_nodes.at(index).childNodes());
    if (config_nodes.length() == 1) {
      // Read modelList / model / configItem.
      const QDomNodeList props(config_nodes.at(0).childNodes());
      for (int props_idx = 0; props_idx < props.length(); props_idx++) {
        const QDomNode prop(props.at(props_idx));
        if (prop.nodeName() == "name") {
          xkb_model.name = prop.toElement().text();
        } else if (prop.nodeName() == "description") {
          xkb_model.description = GetLocalDesc(prop.toElement().text());
        } else if (prop.nodeName() == "vendor") {
          xkb_model.vendor = prop.toElement().text();
        }
      }
    }
    model_list.append(xkb_model);
  }
}

// Read configItem in layoutVariant node.
void ReadLayoutVariantList(const QDomNode& root,
                           XKbLayoutVariantList& variant_list) {
  const QDomNodeList variant_nodes(root.childNodes());
  for (int variant_idx = 0; variant_idx < variant_nodes.length();
       variant_idx++) {
    const QDomNode variant_node(variant_nodes.at(variant_idx));
    XkbLayoutVariant layout_variant;
    const QDomNode config_node(variant_node.firstChild());
    if (config_node.isNull()) {
      continue;
    }
    const QDomNodeList props(config_node.childNodes());
    for (int prop_idx = 0; prop_idx < props.length(); prop_idx++) {
      const QDomNode prop(props.at(prop_idx));
      if (prop.nodeName() == "name") {
        layout_variant.name = prop.toElement().text();
      } else if (prop.nodeName() == "description") {
        layout_variant.description = GetLocalDesc(prop.toElement().text());
      } else if (prop.nodeName() == "shortDescription") {
        layout_variant.short_description = prop.toElement().text();
      } else if (prop.nodeName() == "languageList") {
        // Read layout / configItem / languageList
        const QDomNodeList lang_nodes(prop.childNodes());
        for (int lang_idx = 0; lang_idx < lang_nodes.length(); lang_idx++) {
          const QDomNode lang_node(lang_nodes.at(lang_idx));
          layout_variant.language_list.append(lang_node.toElement().text());
        }
      }
    }
    variant_list.append(layout_variant);
  }
}

// Read configItem in layout node.
void ReadLayoutConfigItem(const QDomNode& root, XkbLayout& xkb_layout) {
  const QDomNodeList props(root.childNodes());
  for (int prop_idx = 0; prop_idx < props.length(); prop_idx++) {
    const QDomNode prop(props.at(prop_idx));
    if (prop.nodeName() == "name") {
      xkb_layout.name = prop.toElement().text();
    } else if (prop.nodeName() == "description") {
      xkb_layout.description = GetLocalDesc(prop.toElement().text());
    } else if (prop.nodeName() == "shortDescription") {
      xkb_layout.short_description = prop.toElement().text();
    } else if (prop.nodeName() == "languageList") {
      // Read layout / configItem / languageList
      const QDomNodeList lang_nodes(prop.childNodes());
      for (int lang_idx = 0; lang_idx < lang_nodes.length(); lang_idx++) {
        const QDomNode lang_node(lang_nodes.at(lang_idx));
        xkb_layout.language_list.append(lang_node.toElement().text());
      }
    }
  }
}

// Read layoutList node.
void ReadLayoutList(const QDomNode& root, XkbLayoutList& layout_list) {
  const QDomNodeList layout_nodes(root.childNodes());
  for (int layout_idx = 0; layout_idx < layout_nodes.length(); layout_idx++) {
    XkbLayout xkb_layout;
    const QDomNodeList content_nodes(layout_nodes.at(layout_idx).childNodes());
    for (int content_idx = 0; content_idx < content_nodes.length();
         content_idx++) {
      const QDomNode content_node(content_nodes.at(content_idx));
      if (content_node.nodeName() == "configItem") {
        // Read layout / configItem
        ReadLayoutConfigItem(content_node, xkb_layout);

      } else if (content_node.nodeName() == "variantList") {
        // Read layout / variantList
        ReadLayoutVariantList(content_node, xkb_layout.variant_list);
      }
    }
    layout_list.append(xkb_layout);
  }
}

// Read xkb config file at |filepath|. Returns false if failed.
bool ReadConfig(const QString& filepath, XkbConfig& config) {
  QDomDocument doc;
  if (!doc.setContent(ReadFile(filepath), true)) {
    return false;
  }

  const QDomElement root = doc.documentElement();
  const QDomNodeList nodes(root.childNodes());

  for (int index = 0; index < nodes.length(); index++) {
    const QDomNode node = nodes.at(index);
    if (node.nodeName() == "modelList") {
      // Read model list.
      ReadModelList(node, config.model_list);
    } else if (node.nodeName() == "layoutList") {
      // Read layout list.
      ReadLayoutList(node, config.layout_list);
    }
  }

  return true;
}

}  // namespace

XkbConfig GetXkbConfig(const QString& locale) {
  // Set locale first.
  (void) setlocale(LC_ALL, (locale + ".UTF-8").toLocal8Bit().constData());

  XkbConfig config;
  if (!ReadConfig(kXkbBaseRule, config)) {
    qWarning() << "Failed to read xkb config file" << kXkbBaseRule;
    // Reset locale.
    (void) setlocale(LC_ALL, kDefaultLang);
    return config;
  }

  // NOTE(xushaohua): Do not read extra keyboard layout list currently.
//  if (!ReadConfig(kXkbExtraRule, config)) {
//    qWarning() << "Failed to read xkb config file" << kXkbExtraRule;
//  }

  (void) setlocale(LC_ALL, kDefaultLang);

  return config;
}

bool SetXkbLayout(const QString& layout) {
  return SpawnCmd(kSetXkbMapCmd, {layout});
}

bool SetXkbLayout(const QString& layout, const QString& variant) {
  return SpawnCmd(kSetXkbMapCmd, {layout, variant});
}

bool SetXkbModel(const QString& model) {
  return SpawnCmd(kSetXkbMapCmd, {"-model", model});
}

}  // namespace installer
